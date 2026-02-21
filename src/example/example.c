
#include "example.h"

#include <string.h>

typedef enum horizontalPosition_t {
    LEFT,
    CENTER,
    RIGHT
} horizontalPosition_t;

typedef enum verticalPosition_t {
    TOP,
    MIDDLE,
    BOTTOM
} verticalPosition_t;

struct example_t {
    int rows;
    int cols;
    bool* cells;
    horizontalPosition_t hpos;
    verticalPosition_t vpos;
};

void destroyExample(example_t** example) {
    if (example && *example) {
        example_t* ex = *example;
        if (ex->cells) free(ex->cells);
        ex->cells = NULL;
        free(ex);
        *example = NULL;
    }
    return;
}

void loadExampleToGrid(grid_t* grid, example_t* example) {
    int cols = grid->cols;
    int rows = grid->rows;

    memset(grid->cells, false, cols * rows * sizeof(bool));

    int startingX = 0;
    int startingY = 0;

    switch (example->hpos) {
        case LEFT: startingX = 0; break;
        case CENTER: startingX = cols / 2 - example->cols / 2; break;
        case RIGHT: startingX = cols - example->cols; break;
    }

    switch (example->vpos) {
        case TOP: startingY = 0; break;
        case MIDDLE: startingY = rows / 2 - example->rows / 2; break;
        case BOTTOM: startingY = rows - example->rows; break;
    }

    for (int y = 0; y < example->rows; y++)
    for (int x = 0; x < example->cols; x++) {
        if ( example->cells[y * example->cols + x] )
            ((bool*)grid->cells)[(startingY + y) * cols + startingX + x] = true;
    }

    info("Loaded example to grid");

    return;

}


example_t* nothing() {
    example_t* e = malloc(sizeof(example_t));
    e->rows = 0;
    e->cols = 0;
    e->hpos = LEFT;
    e->vpos = TOP;
    e->cells = NULL;

    return e;

}

example_t* chaos() {
    example_t* e = malloc(sizeof(example_t));
    e->rows = 3;
    e->cols = 3;
    e->hpos = CENTER;
    e->vpos = MIDDLE;
    e->cells = malloc(e->rows * e->cols * sizeof(bool));

    bool data[3][3] = {
        { 1, 1, 0 },
        { 0, 1, 1 },
        { 0, 1, 0 }
    };

    for (int r = 0; r < e->rows; r++)
    for (int c = 0; c < e->cols; c++)
        e->cells[r * e->cols + c] = data[r][c];

    return e;

}

example_t* diehard() {
    example_t* e = malloc(sizeof(example_t));
    e->rows = 3;
    e->cols = 8;
    e->hpos = CENTER;
    e->vpos = MIDDLE;
    e->cells = malloc(e->rows * e->cols * sizeof(bool));

    bool data[3][8] = {
        { 0, 0, 0, 0, 0, 0, 1, 0},
        { 1, 1, 0, 0, 0, 0, 0, 0},
        { 0, 1, 0, 0, 0, 1, 1, 1}
    };

    for (int r = 0; r < e->rows; r++)
    for (int c = 0; c < e->cols; c++)
        e->cells[r * e->cols + c] = data[r][c];

    return e;

}

example_t* glider() {
    example_t* e = malloc(sizeof(example_t));
    e->rows = 3;
    e->cols = 3;
    e->hpos = LEFT;
    e->vpos = TOP;
    e->cells = malloc(e->rows * e->cols * sizeof(bool));

    bool data[3][3] = {
        { 0, 1, 0},
        { 0, 0, 1},
        { 1, 1, 1}
    };

    for (int r = 0; r < e->rows; r++)
    for (int c = 0; c < e->cols; c++)
        e->cells[r * e->cols + c] = data[r][c];

    return e;

}

example_t* acorn() {
    example_t* e = malloc(sizeof(example_t));
    e->rows = 3;
    e->cols = 7;
    e->hpos = CENTER;
    e->vpos = MIDDLE;
    e->cells = malloc(e->rows * e->cols * sizeof(bool));

    bool data[3][7] = {
        { 0, 1, 0, 0, 0, 0, 0},
        { 0, 0, 0, 1, 0, 0, 0},
        { 1, 1, 0, 0, 1, 1, 1}
    };

    for (int r = 0; r < e->rows; r++)
    for (int c = 0; c < e->cols; c++)
        e->cells[r * e->cols + c] = data[r][c];

    return e;

}

example_t* galaxy() {
    example_t* e = malloc(sizeof(example_t));
    e->rows = 9;
    e->cols = 9;
    e->hpos = CENTER;
    e->vpos = MIDDLE;
    e->cells = malloc(e->rows * e->cols * sizeof(bool));

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

    for (int r = 0; r < e->rows; r++)
    for (int c = 0; c < e->cols; c++)
        e->cells[r * e->cols + c] = data[r][c];

    return e;

}
