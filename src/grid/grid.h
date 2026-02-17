#ifndef GRID_H
#define GRID_H

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

#include "../types/direction.h"

typedef struct {
    void*  cells;
    size_t elem_size;
    int    rows;
    int    cols;
} grid_t;

grid_t makeGrid(int rows, int cols, size_t elem_size, const void* init);
void initializeGrid(grid_t* grid);
void shiftGrid(grid_t grid, direction_t direction);
void freeGrid(grid_t g);

static inline void* grid_at(grid_t* g, int r, int c) {
    return (unsigned char*)g->cells + ((size_t)(r * g->cols + c) * g->elem_size);
}

static inline const void* grid_at_const(const grid_t* g, int r, int c) {
    return (const unsigned char*)g->cells + ((size_t)(r * g->cols + c) * g->elem_size);
}

#define GRID_AT(T, gptr, r, c) ((T*)grid_at((gptr), (r), (c)))

#endif

