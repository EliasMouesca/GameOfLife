#include "grid.h"

grid_t makeCopyGrid(int rows, int cols, size_t elementSize, const void* init) {
    grid_t g;
    g.rows = rows;
    g.cols = cols;
    g.elementSize = elementSize;

    size_t total = (size_t)rows * (size_t)cols * elementSize;

    g.cells = malloc(total);
    if (!g.cells) critical("Out of memory\n");

    if (init)
        memcpy(g.cells, init, total);
    else
        memset(g.cells, 0, total);

    return g;
}

grid_t makeEmptyGrid() {
    return (grid_t){.rows=0, .cols=0,.elementSize=0,.cells=NULL};
}

grid_t makeModelGrid(int rows, int cols, size_t elementSize, const void* model) {
    if (elementSize == 0) critical("Llamada a makeModelGrid() con elementSize = 0");
    grid_t g;
    g.rows = rows;
    g.cols = cols;
    g.elementSize = elementSize;

    size_t total = (size_t)rows * (size_t)cols * elementSize;
    g.cells = malloc(total);

    if (!g.cells) critical("Out of memory\n");

    if (!model){
        memset(g.cells, 0 , total);
        return g;
    }

    for (int i = 0; i < g.cols * g.rows * g.elementSize; i+=g.elementSize)
        memcpy((char*)g.cells+i, model, elementSize);

    return g;

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

    size_t total = (size_t)grid.rows * (size_t)grid.cols * grid.elementSize;

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
            buffer + ((size_t)(y * grid.cols + x) * grid.elementSize);

        if (newX >= 0 && newX < grid.cols &&
            newY >= 0 && newY < grid.rows)
        {
            unsigned char* src_cell =
                src + ((size_t)(newY * grid.cols + newX) * grid.elementSize);

            memcpy(dst_cell, src_cell, grid.elementSize);
        }
    }

    memcpy(grid.cells, buffer, total);
    free(buffer);
}

void freeGrid(grid_t g) {
    if (g.cells) free(g.cells);
}

