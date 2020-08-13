/*
 * CLI frontend for core.c
 *
 * PART OF: echo3d
 * FILENAME: cli.c
 * AUTHOR: Philipp Lensch
 */

#include <stdio.h>
#include <unistd.h>

#include "core.h"

int main(int argc, char ** argv)
{
    if (argc < 2)
    {
        fprintf(stderr, "Usage: %s\n"
                        "Arguments:\n"
                        "-i [input PC]\n"
                        "-p [path PC] (generates normals)\n"
                        "-o [output PC]\n”"
                        "-z colors PC by Z\n"
                        "-c centers PC around (0|0|0)\n"
                        "-h shows this help message\n", argv[0]);
        return(1);
    }

    unsigned int normals=0, color=0, center=0;
    char *inpath, *pathpath, *outpath;

    while (1)
    {
        int result = getopt(argc, argv, "i:p:o:czh");
        if (result == -1) break;
        switch (result)
        {
            case '?':
                return(1);
            case ':':
                return(1);
            case 'h':
            printf("Usage: %s\n"
                    "Arguments:\n"
                    "-i [input PC]\n"
                    "-p [path PC] (generates normals)\n"
                    "-o [output PC]\n”"
                    "-z colors PC by Z\n"
                    "-c centers PC around (0|0|0)\n"
                    "-h shows this help message\n", argv[0]);
                return(0);
            case 'p':
                normals = 1;
                printf("Generating Normals\n");
                pathpath = optarg;
                break;
            case 'i':
                inpath = optarg;
                break;
            case 'o':
                outpath = optarg;
                break;
            case 'c':
                printf("Centering\n");
                center = 1;
                break;
            case 'z':
                printf("Coloring\n");
                color = 1;
                break;
            default:
                break;
        }
    }

    generateNormals(inpath, (normals) ? pathpath : "", outpath, color, center, normals);
}
