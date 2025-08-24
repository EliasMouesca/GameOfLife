#ifndef EXAMPLES_H
#define EXAMPLES_H

#include "grid.h"

#include <stdbool.h>

typedef enum {
    LEFT,
    CENTER,
    RIGHT
} horizontalPosition_t;

typedef enum {
    TOP,
    MIDDLE,
    BOTTOM
} verticalPosition_t;

typedef struct {
    int rows;
    int cols;
    bool* cells;
    horizontalPosition_t hpos;
    verticalPosition_t vpos;
} example_t;

void loadExample(grid_t grid, example_t example);
void destroyExample(example_t* example);

example_t chaos();
example_t diehard();
example_t glider();
example_t acorn();
example_t galaxy();

#endif
