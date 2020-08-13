/*
 * Generates pseudo-normals from multibeam-echosounder point clouds and the echosounder location over time.
 *
 * PART OF: echo3d
 * FILENAME: core.h
 * AUTHOR: Philipp Lensch
 */

#ifndef CORE_H
#define CORE_H

int generateNormals(char* infile, char* pathfile, char* outfile, unsigned int color, unsigned int center,
                    unsigned int normals);

#endif // CORE_H
