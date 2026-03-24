#ifndef PGM_H
#define PGM_H

#include <stdio.h>
#include <stdbool.h>

#define WIDTH 200
#define HEIGHT 200
#define GREYDEPTH 255

// retrieve header from given pgm file (first 14 chars with a null character)
char *getHeader(char *fileName);

// split header into list of data e.g. {"P5", "200", "200", "255"}
char **splitHeader(char *header);

// check if given pgm header indicates p5 format, has correct dimensions, and correct max grey value
bool isValidHeader(char **header);

// check if given file has pgm extension
bool isPGM(char *fileName);

// generate sk file that matches given pgm file name
char *getSkName(char *fileName);

// opens pgm and creates sk file, calls function to write the sk file
// returns whether write was successful or not
bool processPGM(char *fileName, char **headerList);

#endif
