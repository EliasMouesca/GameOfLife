#include "grid.h"

grid_t makeGrid(int rows, int cols, bool* init) {
    grid_t g;
    g.rows = rows;
    g.cols = cols;
    g.cells = malloc(rows * cols * sizeof(bool));
    if (!g.cells) {
        fprintf(stderr, "Out of memory\n");
        exit(1);
    }
    memcpy(g.cells, init, rows * cols * sizeof(bool));
    return g;
}

void shiftGrid(grid_t grid, direction_t direction) {
    int dx, dy;

    switch (direction) {
        case DIRECTION_UP:
            dx = 0, dy = 1;
            break;

        case DIRECTION_LEFT:
            dx = 1, dy = 0;
            break;

        case DIRECTION_DOWN:
            dx = 0, dy = -1;
            break;

        case DIRECTION_RIGHT:
            dx = -1, dy = 0;
            break;
        default:
            dx = 0, dy = 0;
            break;
    }

    bool* buffer = (bool*) malloc(grid.rows * grid.cols * sizeof(bool));

    for (int y = 0; y < grid.rows; y++)
    for (int x = 0; x < grid.cols; x++) {
        int newX = x + dx, newY = y + dy;
        buffer[y * grid.cols + x] = ((newX >= 0 && newX < grid.cols) && (newY >= 0 && newY < grid.rows)) ? grid.cells[newY * grid.cols + newX] : false;
    }

    memcpy(grid.cells, buffer, grid.rows * grid.cols * sizeof(bool));

    free(buffer);

    return;
}


void freeGrid(grid_t g) {
    free(g.cells);
}



