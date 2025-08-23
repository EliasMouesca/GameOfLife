
#include "examples.h"

void loadExample(grid_t grid, example_t example) {
    memset(grid.cells, false, grid.cols * grid.rows * sizeof(bool));

    int startingX;
    int startingY;

    switch (example.hpos) {
        case LEFT: startingX = 0; break;
        case CENTER: startingX = grid.cols / 2 - example.cols / 2; break;
        case RIGHT: startingX = grid.cols - example.cols; break;
    }

    switch (example.vpos) {
        case TOP: startingY = 0; break;
        case MIDDLE: startingY = grid.rows / 2 - example.rows / 2; break;
        case BOTTOM: startingY = grid.rows - example.rows; break;
    }

    for (int y = 0; y < example.rows; y++)
    for (int x = 0; x < example.cols; x++) {
        if ( example.cells[y * example.cols + x] )
            grid.cells[(startingY + y) * grid.cols + startingX + x] = true;
    }

    return;

}

void destroyExample(example_t* example) {
    free(example->cells);
    example->cells = NULL;
    return;
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
    e.hpos = TOP;
    e.vpos = LEFT;
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
