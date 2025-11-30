#ifndef GRID_H
#define GRID_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "direction.h"

typedef struct {
    bool* cells;
    int rows;
    int cols;
} grid_t;

grid_t makeGrid(int rows, int cols, bool* init);
void shiftGrid(grid_t grid, direction_t direction);

void freeGrid(grid_t g);

#endif
