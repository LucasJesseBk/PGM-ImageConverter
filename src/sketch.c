// Basic program skeleton for a Sketch File (.sk) Viewer
#include "displayfull.h"
#include "sketch.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Allocate memory for a drawing state and initialise it
state *newState(void) {
    state *s = malloc(sizeof(state));
    s->x = 0;
    s->y = 0;
    s->tx = 0;
    s->ty = 0;
    s->tool = LINE;
    s->start = 0;
    s->data = 0;
    s->end = 0;
    return s;
}

// Release all memory associated with the drawing state
void freeState(state *s) {
    free(s);
}

// Extract an opcode from a byte (two most significant bits).
int getOpcode(byte b) {
    return b >>= 6;
} 

// Extract an operand (-32..31) from the rightmost 6 bits of a byte.
int getOperand(byte b) {
    int c = (b & (63));
    if (c >= 32) {
        c = c - 64;
    }
    return c;
}

// Execute the next byte of the command sequence.
void obey(display *d, state *s, byte op) {
    int opcode = getOpcode(op);
    int operand = getOperand(op);

    if (opcode == DX) {
        s->tx = s->tx + operand;
    }
    else if (opcode == DY) {
        s->ty = s->ty + operand;
        if (s->tool == LINE) line(d, s->x, s->y, s->tx, s->ty);
        if (s->tool == BLOCK) block(d, s->x, s->y, s->tx - s->x, s->ty - s->y);
        s->x = s->tx;
        s->y = s->ty;
    }
    else if (opcode == TOOL) {
        if (operand == 0) s->tool = NONE;
        else if (operand == 1) s->tool = LINE;
        else if (operand == 2) s->tool = BLOCK;
        else if (operand == 3) colour(d, s->data);
        else if (operand == 4) s->tx = s->data;
        else if (operand == 5) s->ty = s->data;
        else if (operand == 6) show(d);
        else if (operand == 7) pause(d, s->data);
        else if (operand == 8) s->end = true;
        s->data = 0;
    }
    else if (opcode == DATA) {
        s->data <<= 6;
        if (operand < 0) operand = operand + 64;
        s->data = s->data + operand;
    }
}

// Draw a frame of the sketch file
bool processSketch(display *d, const char pressedKey, void *data) {
    // current command
    int curCom = 0;

    if (data == NULL) return (pressedKey == 27);

    state *s = (state*) data;
    FILE *sketch = fopen(getName(d), "rb");

    if (!sketch) {
        printf("couldn't open file");
        return true;
    }

    unsigned char b = fgetc(sketch);
    while (!feof(sketch) && s->end == false) {
        if (curCom >= s->start) {
            obey(d, s, b);
            s->start = s->start + 1;
        }
        curCom = curCom + 1;
        b = fgetc(sketch);
    }

    // if a sketch is completed reset the start value
    if (feof(sketch)) s->start = 0;

    show(d);
    fclose(sketch);

    s->x = 0;
    s->y = 0;
    s->tx = 0;
    s->ty = 0;
    s->tool = LINE;
    s->data = 0;
    s->end = false;

    return (pressedKey == 27);
}

// View a sketch file in a 200x200 pixel window given the filename
void view(char *filename) {
    display *d = newDisplay(filename, 200, 200);
    state *s = newState();
    run(d, s, processSketch);
    freeState(s);
    freeDisplay(d);
}

// Include a main function only if we are not testing (make sketch),
// otherwise use the main function of the test.c file (make test).
#ifndef TESTING
int main(int n, char *args[n]) {
    if (n != 2) { // return usage hint if not exactly one argument
        printf("Use ./sketch file\n");
        exit(1);
    }
    else view(args[1]); // otherwise view sketch file in argument
    return 0;
}
#endif
