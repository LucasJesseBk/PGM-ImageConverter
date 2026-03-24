#include "pgm.h"
#include <stdio.h>
#include <stdlib.h>

void runTests(void);

int main(int n, char *args[n]) {
    if (n == 1) runTests();
    
    else if (n == 2) {
        char *header = getHeader(args[1]);
        char **headerList = splitHeader(header);

        if (isValidHeader(headerList) && isPGM(args[1])) {
            if (!processPGM(args[1], headerList)) {
                printf(
                    "Sk file not generated, invalid number of grey values\n"
                    "Expected 40000 values (200*200 size)\n"
                );
            }
        }
        else printf("Invalid pgm\n");

        free(header);
        free(headerList);
    }
    else {
        printf("Usage:\n./converter filename.pgm\n");
    }
}
