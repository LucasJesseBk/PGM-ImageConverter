#include "pgm.h"
#include <stdlib.h>
#include <string.h>

// retrieve header from given pgm file (first 14 chars with a null character)
char *getHeader(char *fileName) {
    const int maxHeaderLength = 15;
    char *header = malloc(maxHeaderLength * sizeof(char));
    FILE *pgm = fopen(fileName, "rb");

    // if file not found return invalid header
    if (pgm == NULL) {
        header[0] = '1';
        header[1] = '\0';
        return header;
    }

    unsigned char b = fgetc(pgm);

    // only add to header if file isn't empty
    if (!feof(pgm)) {
        // add first element of header
        header[0] = b;
        int i = 1;

        // add next 13 elements of header
        for (i = 1; i < maxHeaderLength - 1; i++) {
            b = fgetc(pgm);
            if (feof(pgm)) break;
            header[i] = b;
        }

        // add null character to header
        header[i] = '\0';
    }
    // if file is empty return invalid header
    else {
        header[0] = '1';
        header[1] = '\0';
        return header;
    }

    fclose(pgm);
    return header;
}

// split header into list of data e.g. {"P5", "200", "200", "255"}
char **splitHeader(char *header) {
    int dataNumber = 0;
    const int maxDataItems = 4;

    // dynamically allocated memory for list of 4 headers
    char **headers = malloc(sizeof(char *) * maxDataItems);
    const char spaces[] = " ";
    char *token;

    token = strtok(header, spaces);
    while (token != NULL) {
        headers[dataNumber] = token;
        token = strtok(NULL, spaces);
        dataNumber++;
    }

    return headers;
}

// check if given pgm header indicates p5 format, has correct dimensions, and correct max grey value
bool isValidHeader(char **header) {
    // only accept P5 format of pgm
    if (strcmp(header[0], "P5") != 0) return false;

    // only accept width/height values of 200
    if ((atoi(header[1]) != WIDTH) || (atoi(header[2]) != HEIGHT)) return false;

    // only accept 8bit grey value
    if (atoi(header[3]) != GREYDEPTH) return false;

    return true;
}

// check if given file has pgm extension
bool isPGM(char *fileName) {
    int i = 0;
    for (i = 0; fileName[i] != '.'; i++)
        if (fileName[i] == '\0') return false;

    if (i != 0 &&
        (fileName[i] == '.') &&
        (fileName[i + 1] == 'p') &&
        (fileName[i + 2] == 'g') &&
        (fileName[i + 3] == 'm'))
        return true;

    return false;
}

// generate sk file that matches given pgm file name
char *getSkName(char *fileName) {
    char *baseName = malloc(strlen(fileName) + 4); // +4 for ".sk" and null character
    int i = 0;

    for (i = 0; fileName[i] != '.'; i++) {
        baseName[i] = fileName[i];
    }

    // add null pointer to end of basename
    baseName[i] = '\0';
    strcat(baseName, ".sk");

    return baseName;
}
