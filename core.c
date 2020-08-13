/*
 * Generates pseudo-normals from multibeam-echosounder point clouds and the echosounder location over time.
 *
 * PART OF: echo3d
 * FILENAME: core.c
 * AUTHOR: Philipp Lensch
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "core.h"

typedef struct
{
    double x;
    double y;
    double z;
    double nx;
    double ny;
    double nz;
    unsigned char r;
    unsigned char g;
    unsigned char b;
} point;

typedef struct
{
    unsigned long count;
    double maxX;
    double maxY;
    double maxZ;
    double minX;
    double minY;
    double minZ;

    point* points;
} pointcloud;


void normal(point* p, point* path)
{
    double x = path->x - p->x;
    double y = path->y - p->y;
    double z = path->z - p->z;
    double length = sqrt(x*x + y*y + z*z);

    p->nx = x / length;
    p->ny = y / length;
    p->nz = z / length;
}

void setNormals(pointcloud* cloud, pointcloud* path)
{
    for (unsigned long a = 0; a < cloud->count; a++)
    {
        unsigned long pathIndex = 0;
        double tmpLength = 9999; //HACK

        // HACK
        point* ppoints = cloud->points;
        point* ppath = path->points;

        for (unsigned long b = 0; b < path->count; b++)
        {
           double x = ppath[b].x - ppoints[a].x;
           double y = ppath[b].y - ppoints[a].y;
           double z = ppath[b].z - ppoints[a].z;
           double length = sqrt(x*x + y*y + z*z);

           if ( length < tmpLength )
           {
               tmpLength = length;
               pathIndex = b;
           }
        }

        // set normal
        normal(&ppoints[a], &ppath[pathIndex]);
    }
}

int readPoints(FILE* pointFile, pointcloud* cloud)
{

    // count number of points
    cloud->count = 0;
    char tmpch = 0;

    while (!feof(pointFile))
    {
        tmpch = fgetc(pointFile);
        if (tmpch == '\n')
        {
            cloud->count++;
        }
    }

    // rewind to read from top
    rewind(pointFile);

    // status output
    printf("%lu points read\n", cloud->count);

    // allocate memory for point array
    point* points = malloc(sizeof(point)*cloud->count);

    // read in points (unsafe)
    point tmpPoint;
    tmpPoint.nx = 0;
    tmpPoint.ny = 0;
    tmpPoint.nz = 0;

    tmpPoint.r = 0;
    tmpPoint.g = 0;
    tmpPoint.b = 0;

    unsigned long tmpCount = 0;

    // inital point (for min/max comparison)
    fscanf(pointFile,"%lf %lf %lf%*[^\n]\n", &tmpPoint.x, &tmpPoint.y, &tmpPoint.z);
    points[tmpCount] = tmpPoint;
    tmpCount++;

    cloud->maxX = tmpPoint.x; 
    cloud->maxY = tmpPoint.y; 
    cloud->maxZ = tmpPoint.z; 
    cloud->minX = tmpPoint.x; 
    cloud->minY = tmpPoint.y; 
    cloud->minZ = tmpPoint.z; 
    

    while (fscanf(pointFile,"%lf %lf %lf%*[^\n]\n", &tmpPoint.x, &tmpPoint.y, &tmpPoint.z) == 3)
    {
        points[tmpCount] = tmpPoint;
        tmpCount++;

        // compare with min/max values
        if (tmpPoint.x > cloud->maxX) cloud->maxX = tmpPoint.x;
        if (tmpPoint.y > cloud->maxY) cloud->maxY = tmpPoint.y;
        if (tmpPoint.z > cloud->maxZ) cloud->maxZ = tmpPoint.z;

        if (tmpPoint.x < cloud->minX) cloud->minX = tmpPoint.x;
        if (tmpPoint.y < cloud->minY) cloud->minY = tmpPoint.y;
        if (tmpPoint.z < cloud->minZ) cloud->minZ = tmpPoint.z;
    }

    // "link" points to pointcloud struct
    cloud->points = points;

    return 0;
}

void writePLY (FILE* fp, pointcloud* cloud)
{

    // set up .ply file header
    fprintf(fp, "ply\n");
    fprintf(fp, "format ascii 1.0\n");
    fprintf(fp, "comment computed normals and colored by depth\n");
    fprintf(fp, "element vertex %lu\n"
                "property double x\n"
                "property double y\n"
                "property double z\n"
                "property double nx\n"
                "property double ny\n"
                "property double nz\n"
                "property uchar red\n"
                "property uchar green\n"
                "property uchar blue\n"
                "end_header\n",
                cloud->count);

    // write points (x y z nx ny nz r g b)
    point* points = cloud->points;

    for (unsigned long i = 0; i < cloud->count; i++)
    {
        fprintf(fp, "%f %f %f %f %f %f %u %u %u\n", points[i].x, points[i].y, points[i].z,
                                           points[i].nx, points[i].ny, points[i].nz,
                                           points[i].r,points[i].g,points[i].b);
    }
}

void infoPointCloud (pointcloud* cloud)
{
    printf("PointCount: %lu\n", cloud->count);
    printf("maxX %f maxY %f maxZ %f\n", cloud->maxX, cloud->maxY, cloud->maxZ);
    printf("minX %f minY %f minZ %f\n", cloud->minX, cloud->minY, cloud->minZ);
}

void centerPointCloud (pointcloud* cloud)
{
    double dx = (cloud->maxX + cloud->minX) / 2;
    double dy = (cloud->maxY + cloud->minY) / 2;
    double dz = (cloud->maxZ + cloud->minZ) / 2;

    // move points
    for (unsigned long i = 0; i < cloud->count; i++)
    {
        cloud->points[i].x = cloud->points[i].x - dx;
        cloud->points[i].y = cloud->points[i].y - dy;
        cloud->points[i].z = cloud->points[i].z - dz;
    }

    // set min/max values
    cloud->maxX = cloud->maxX - dx;
    cloud->maxY = cloud->maxY - dy;
    cloud->maxZ = cloud->maxZ - dz;
    cloud->minX = cloud->minX - dx;
    cloud->minY = cloud->minY - dy;
    cloud->minZ = cloud->minZ - dz;
}

void colorByZPointCloud (pointcloud* cloud)
{
    for (unsigned long i = 0; i < cloud->count; i++)
    {
        cloud->points[i].g = 0;
        cloud->points[i].r = (cloud->points[i].z - cloud->minZ) / (cloud->maxZ - cloud->minZ) * 255;
        cloud->points[i].b = 255 - ((cloud->points[i].z - cloud->minZ) / (cloud->maxZ - cloud->minZ) * 255);
    }
}

int generateNormals(char* infile, char* pathfile, char* outfile, unsigned int color, unsigned int center,
                    unsigned int normals)
{
    printf("%s -> %s\n", infile, outfile);
    // read points
    FILE* pointFile = fopen(infile,"r");
    if (pointFile == 0) return 1; 
    
    pointcloud cloud;
    pointcloud path;
    readPoints(pointFile, &cloud);
    fclose(pointFile);

    if (normals == 1)
    {
    // read path
    FILE* pathFile = fopen(pathfile,"r");
    if (pathFile == 0) {
        free(cloud.points);
        return 1;
    }
    readPoints(pathFile, &path);
    fclose(pathFile);

    // find and set normals
    setNormals(&cloud, &path);
    }

    // center point cloud around (0|0|0)
    if (center == 1)
    {
        centerPointCloud(&cloud);
    }

    // color by depth
    if (color == 1)
    {
        colorByZPointCloud(&cloud);
    }

    // output ply
    FILE* outputFile = fopen(outfile, "w");
    if (outputFile == 0) {
        free(cloud.points);
        if (normals == 1) free(path.points);
        return 1;
    }

    writePLY(outputFile, &cloud);
    fclose(outputFile);

    // free memory
    free(cloud.points);
    if (normals == 1) free(path.points);

    return 0;
}
