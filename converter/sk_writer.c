#include "pgm.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>

// ==================================================
// Sketch command definitions
// ==================================================

// convert 6-bit payloads to sketch colour bytes
// 11000000
#define SK_COLOR_PREFIX      0xC0

// use block tool
// 10000010
#define SK_USE_BLOCK_TOOL    0x82

// change colour to current data value (rgba)
// 10000011
#define SK_SET_RGBA          0x83

// TARGETX move to x = 0
// 10000100
#define SK_TARGETX_ZERO      0x84

// move dy down by one pixel
// 01000001
#define SK_MOVE_DOWN_1       0x41

// move up one
// 01111111
#define SK_MOVE_UP_1         0x7F

// full alpha
// 11111111
#define SK_ALPHA_FULL        0xFF

// max horizontal move (5 bits)
// 00011111
#define SK_MOVE_X_MAX        31

// helper for colour payload
#define SK_COLOR_PAYLOAD(x)  (SK_COLOR_PREFIX + (x))

// ==================================================
// convert 8-bit grey value to equivalent rgba value
// and add it to the current states data. 6 bits at a time
// ==================================================
static void setNewColour(FILE *sk, unsigned char greyValue) {
    // take grey value and set r, g and b value to that grey value
    int rgb = (greyValue << 16) + (greyValue << 8) + greyValue;

    // generate 6bit values to add to current states data that reflect grey rgb value
    unsigned char first2 = rgb >> 22;
    unsigned char first6 = (rgb & 0x3F0000) >> 16;
    unsigned char second6 = (rgb & 0xFC00) >> 10;
    unsigned char third6 = (rgb & 0x3F0) >> 4;
    unsigned char last4AndTwo1s = ((rgb & 0xF) << 2) + 0x03;

    // set data to contain wanted grey rgba value
    // 11000000
    fputc(SK_COLOR_PAYLOAD(first2), sk);
    fputc(SK_COLOR_PAYLOAD(first6), sk);
    fputc(SK_COLOR_PAYLOAD(second6), sk);
    fputc(SK_COLOR_PAYLOAD(third6), sk);

    // append last section of rgb value with two ones so that alpha value can be set to '11111111'
    fputc(SK_COLOR_PAYLOAD(last4AndTwo1s), sk);
    fputc(SK_ALPHA_FULL, sk);

    // change colour to current data value (rgba)
    // 10000011
    fputc(SK_SET_RGBA, sk);
}

// draw a block by writing the block sketch command
static void drawBlock(FILE *sk) {
    // move down by current y value and draw block
    // 01000001
    fputc(SK_MOVE_DOWN_1, sk);

    // move up one
    // 01111111
    fputc(SK_MOVE_UP_1, sk);
}

static void newLine(FILE *sk) {
    // move dy down by one pixel
    // 01000001
    fputc(SK_MOVE_DOWN_1, sk);

    // TARGETX move to x = 0
    // 10000100
    fputc(SK_TARGETX_ZERO, sk);
}

// move dx by a given blockwidth 5 bits at a time
static void moveBlockWidth(FILE *sk, int blockWidth) {
    int remainder = blockWidth % SK_MOVE_X_MAX;

    blockWidth -= remainder;
    for (int loops = blockWidth / SK_MOVE_X_MAX; loops > 0; loops--) {
        fputc(SK_MOVE_X_MAX, sk);
    }

    // only add remaining width value if not 0, to avoid redundant writes to sk file
    if (remainder != 0) fputc(remainder, sk);
}

static bool writeSK(FILE *pgm, FILE *sk, int width, int height) {
    // initialise previous colour as impossible value so that the desired colour is set at the start
    int prevColour = 256;
    int blockWidth = 0;

    // use block tool in sketch file
    // 10000010
    fputc(SK_USE_BLOCK_TOOL, sk);

    // read over each pixel in the pgm
    for (int y = 1; y <= height; y++) {
        for (int x = 1; x <= width; x++) {
            unsigned char b = fgetc(pgm);

            // if end of file reached earlier than expected return false (fail)
            if (feof(pgm)) return false;

            // if current colour has changed or end of line is reached or new line has started
            if ((prevColour != b) || x == width || x == 1) {
                if (x == width) moveBlockWidth(sk, blockWidth + 1);
                else moveBlockWidth(sk, blockWidth);

                drawBlock(sk);
                blockWidth = 1;

                // only set new colour if colour has actually changed
                if (prevColour != b) setNewColour(sk, b);
            }
            else blockWidth++;

            prevColour = b;
        }

        blockWidth = 0;
        newLine(sk);
    }

    if (fgetc(pgm) && feof(pgm)) return true;
    else return false;
}

// opens pgm and creates sk file, calls function to write the sk file
// returns whether write was successful or not
bool processPGM(char *fileName, char **headerList) {
    bool dataValid;
    char *skName = getSkName(fileName);

    FILE *pgm = fopen(fileName, "rb");
    FILE *sk = fopen(skName, "wb");

    while (fgetc(pgm) != '\n');

    int width = WIDTH;
    int height = HEIGHT;

    dataValid = writeSK(pgm, sk, width, height);

    fclose(pgm);
    fclose(sk);

    if (dataValid) printf("File %s has been written.\n", skName);
    else remove(skName);

    free(skName);
    return dataValid;
}
