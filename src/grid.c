#include "grid.h"

grid_t makeGrid(int rows, int cols, bool init[rows][cols]) {
    grid_t g;
    g.rows = rows;
    g.cols = cols;
    g.cells = malloc(rows * cols * sizeof(bool));
    if (!g.cells) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    memcpy(g.cells, &init[0][0], rows * cols * sizeof(bool));
    return g;
}

void freeGrid(grid_t g) {
    free(g.cells);
}


