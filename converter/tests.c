#include "pgm.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

void assert(int lineNumber, bool a) {
    if (a) return;
    printf(
        "The test on line %d fails.\n"
        "Check if test is dependent on having fractal.pgm or bands.pgm in folder\n",
        lineNumber
    );
    exit(1);
}

void testGetHeader(void) {
    char *testHeader = getHeader("images/fractal.pgm");
    assert(__LINE__, strcmp(testHeader, "P5 200 200 255") == 0);
    free(testHeader);

    testHeader = getHeader("images/bands.pgm");
    assert(__LINE__, strcmp(testHeader, "P5 200 200 255") == 0);
    free(testHeader);

    testHeader = getHeader("images/fractal.pg");
    assert(__LINE__, strcmp(testHeader, "1") == 0);
    free(testHeader);

    testHeader = getHeader("");
    assert(__LINE__, strcmp(testHeader, "1") == 0);
    free(testHeader);
}

void testIsValidHeader(void) {
    char *headerList[4] = {"P5", "200", "200", "255"};
    assert(__LINE__, isValidHeader(headerList) == true);
}

void testIsPGM(void) {
    assert(__LINE__, isPGM("images/file.pgm") == true);
    assert(__LINE__, isPGM("images/file.txt") == false);
}

void testGetSkName(void) {
    char *skName = getSkName("images/file.pgm");
    assert(__LINE__, strcmp(skName, "images/file.sk") == 0);
    free(skName);
}

void runTests(void) {
    testGetHeader();
    testIsValidHeader();
    testIsPGM();
    testGetSkName();
    printf("Tests passed, yay!\n");
}
