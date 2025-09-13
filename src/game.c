
#include "game.h"

void update(grid_t grid) {
    bool* buffer = (bool*) malloc(grid.rows * grid.cols * sizeof(bool));

    for (int y = 0; y < grid.rows; y++)
    for (int x = 0; x < grid.cols; x++) {
        int i = y * grid.cols + x;
        int neighborsAlive = 0;

        if ((y > 0 && x > 0) ? grid.cells[i-grid.cols-1] : false) neighborsAlive++;
        if ((y > 0) ? grid.cells[i-grid.cols] : false) neighborsAlive++;
        if ((y > 0 && x < grid.cols - 1) ? grid.cells[i-grid.cols + 1] : false) neighborsAlive++;
        
        if ((x > 0) ? grid.cells[i-1] : false) neighborsAlive++;
        if ((x < grid.cols - 1) ? grid.cells[i+1] : false) neighborsAlive++;

        if ((y < grid.rows - 1 && x > 0) ? grid.cells[i+grid.cols-1] : false) neighborsAlive++;
        if ((y < grid.rows - 1) ? grid.cells[i+grid.cols] : false) neighborsAlive++;
        if ((y < grid.rows - 1 && x < grid.cols - 1) ? grid.cells[i+grid.cols+1] : false) neighborsAlive++;

        if (grid.cells[i]) {
            if (neighborsAlive == 2 || neighborsAlive == 3) buffer[i] = true;
            else buffer[i] = false;
        } else {
            if (neighborsAlive == 3) buffer[i] = true;
            else buffer[i] = false;
        }
    }

    memcpy(grid.cells, buffer, grid.rows * grid.cols * sizeof(bool));

    free(buffer);

    return;
}

void draw(SDL_Renderer* renderer, grid_t grid, int blockSize, Sint32 mouseX, Sint32 mouseY) {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 0xff, 0xff, 0xff, 0xff);
    for (int y = 0; y < grid.rows; y++)
    for (int x = 0; x < grid.cols; x++)
        if (grid.cells[y * grid.cols + x]) {
            SDL_Rect rect = {x * blockSize, y * blockSize, blockSize, blockSize};
            SDL_RenderFillRect(renderer, &rect);
        }

    if ( (mouseX > 0 && mouseX < grid.cols * blockSize) && 
            (mouseY > 0 && mouseY < grid.rows * blockSize) )
    {
        int x = mouseX - (mouseX % blockSize);
        int y = mouseY - (mouseY % blockSize);

        SDL_Rect rect = {x, y, blockSize, blockSize};
        SDL_SetRenderDrawColor(renderer, 0xaa, 0xaa, 0xaa, 0xff);
        SDL_RenderFillRect(renderer, &rect);
    }

    SDL_RenderPresent(renderer);

    return;

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


