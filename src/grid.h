#ifndef GRID_H
#define GRID_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

typedef struct {
    bool* cells;
    int rows;
    int cols;
} grid_t;

grid_t makeGrid(int rows, int cols, bool init[rows][cols]);

void freeGrid(grid_t g);

#endif
