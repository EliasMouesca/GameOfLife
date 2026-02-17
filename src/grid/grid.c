#include "grid.h"

grid_t makeGrid(int rows, int cols, size_t elem_size, const void* init) {
    grid_t g;
    g.rows = rows;
    g.cols = cols;
    g.elem_size = elem_size;

    size_t total = (size_t)rows * (size_t)cols * elem_size;

    g.cells = malloc(total);
    if (!g.cells) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    if (init)
        memcpy(g.cells, init, total);
    else
        memset(g.cells, 0, total);

    return g;
}

void initializeGrid(grid_t* grid) {
    grid->cells = NULL;
    grid->elem_size = 0;
    grid->rows = 0;
    grid->cols = 0;
}

void shiftGrid(grid_t grid, direction_t direction) {
    int dx, dy;

    switch (direction) {
        case DIRECTION_UP:    dx = 0;  dy = 1;  break;
        case DIRECTION_LEFT:  dx = 1;  dy = 0;  break;
        case DIRECTION_DOWN:  dx = 0;  dy = -1; break;
        case DIRECTION_RIGHT: dx = -1; dy = 0;  break;
        default:              dx = 0;  dy = 0;  break;
    }

    size_t total = (size_t)grid.rows * (size_t)grid.cols * grid.elem_size;

    unsigned char* buffer = malloc(total);
    if (!buffer) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }

    memset(buffer, 0, total);

    unsigned char* src = (unsigned char*)grid.cells;

    for (int y = 0; y < grid.rows; y++)
    for (int x = 0; x < grid.cols; x++) {
        int newX = x + dx;
        int newY = y + dy;

        unsigned char* dst_cell =
            buffer + ((size_t)(y * grid.cols + x) * grid.elem_size);

        if (newX >= 0 && newX < grid.cols &&
            newY >= 0 && newY < grid.rows)
        {
            unsigned char* src_cell =
                src + ((size_t)(newY * grid.cols + newX) * grid.elem_size);

            memcpy(dst_cell, src_cell, grid.elem_size);
        }
    }

    memcpy(grid.cells, buffer, total);
    free(buffer);
}

void freeGrid(grid_t g) {
    free(g.cells);
}

