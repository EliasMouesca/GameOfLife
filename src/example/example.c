
#include "example.h"

void destroyExample(example_t* example) {
    free(example->cells);
    example->cells = NULL;
    return;
}

example_t nothing() {
    example_t e;
    e.rows = 0;
    e.rows = 0;
    e.hpos = LEFT;
    e.vpos = TOP;
    e.cells = NULL;

    return e;

}

example_t chaos() {
    example_t e;
    e.rows = 3;
    e.cols = 3;
    e.hpos = CENTER;
    e.vpos = MIDDLE;
    e.cells = malloc(e.rows * e.cols * sizeof(bool));

    bool data[3][3] = {
        { 1, 1, 0 },
        { 0, 1, 1 },
        { 0, 1, 0 }
    };

    for (int r = 0; r < e.rows; r++)
    for (int c = 0; c < e.cols; c++)
        e.cells[r * e.cols + c] = data[r][c];

    return e;

}

example_t diehard() {
    example_t e;
    e.rows = 3;
    e.cols = 8;
    e.hpos = CENTER;
    e.vpos = MIDDLE;
    e.cells = malloc(e.rows * e.cols * sizeof(bool));

    bool data[3][8] = {
        { 0, 0, 0, 0, 0, 0, 1, 0},
        { 1, 1, 0, 0, 0, 0, 0, 0},
        { 0, 1, 0, 0, 0, 1, 1, 1}
    };

    for (int r = 0; r < e.rows; r++)
    for (int c = 0; c < e.cols; c++)
        e.cells[r * e.cols + c] = data[r][c];

    return e;

}

example_t glider() {
    example_t e;
    e.rows = 3;
    e.cols = 3;
    e.hpos = LEFT;
    e.vpos = TOP;
    e.cells = malloc(e.rows * e.cols * sizeof(bool));

    bool data[3][3] = {
        { 0, 1, 0},
        { 0, 0, 1},
        { 1, 1, 1}
    };

    for (int r = 0; r < e.rows; r++)
    for (int c = 0; c < e.cols; c++)
        e.cells[r * e.cols + c] = data[r][c];

    return e;

}

example_t acorn() {
    example_t e;
    e.rows = 3;
    e.cols = 7;
    e.hpos = CENTER;
    e.vpos = MIDDLE;
    e.cells = malloc(e.rows * e.cols * sizeof(bool));

    bool data[3][7] = {
        { 0, 1, 0, 0, 0, 0, 0},
        { 0, 0, 0, 1, 0, 0, 0},
        { 1, 1, 0, 0, 1, 1, 1}
    };

    for (int r = 0; r < e.rows; r++)
    for (int c = 0; c < e.cols; c++)
        e.cells[r * e.cols + c] = data[r][c];

    return e;

}

example_t galaxy() {
    example_t e;
    e.rows = 9;
    e.cols = 9;
    e.hpos = CENTER;
    e.vpos = MIDDLE;
    e.cells = malloc(e.rows * e.cols * sizeof(bool));

    bool data[9][9] = {
        { 1, 1, 1, 1, 1, 1, 0, 1, 1 },
        { 1, 1, 1, 1, 1, 1, 0, 1, 1 },
        { 0, 0, 0, 0, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 0, 0, 1, 1 },
        { 1, 1, 0, 0, 0, 0, 0, 0, 0 },
        { 1, 1, 0, 1, 1, 1, 1, 1, 1 },
        { 1, 1, 0, 1, 1, 1, 1, 1, 1 }
    };

    for (int r = 0; r < e.rows; r++)
    for (int c = 0; c < e.cols; c++)
        e.cells[r * e.cols + c] = data[r][c];

    return e;

}
