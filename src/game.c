
#include "game.h"
#include "pthread.h"

void update(game_t* game) {
    pthread_mutex_lock(&game->gridLock);
    grid_t grid;
    memcpy(&grid, &(game->grid), sizeof(grid_t));
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
    pthread_mutex_unlock(&game->gridLock);

    free(buffer);

    return;
}

void draw(game_t* game, Sint32 mouseX, Sint32 mouseY) {
    int rows = game->grid.rows;
    int cols = game->grid.cols;
    int blockSize = game->blockSize;
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(game->renderer);

    pthread_mutex_lock(&game->gridLock);
    grid_t grid = makeGrid(rows, cols, (bool*) game->grid.cells);
    pthread_mutex_unlock(&game->gridLock);

    SDL_SetRenderDrawColor(game->renderer, 0xff, 0xff, 0xff, 0xff);
    for (int y = 0; y < rows; y++)
    for (int x = 0; x < cols; x++)
        if (grid.cells[y * cols + x]) {
            SDL_Rect rect = {x * blockSize, y * blockSize, blockSize, blockSize};
            SDL_RenderFillRect(game->renderer, &rect);
        }

    if ( (mouseX > 0 && mouseX < grid.cols * blockSize) && 
            (mouseY > 0 && mouseY < grid.rows * blockSize) )
    {
        int x = mouseX - (mouseX % blockSize);
        int y = mouseY - (mouseY % blockSize);

        SDL_Rect rect = {x, y, blockSize, blockSize};
        SDL_SetRenderDrawColor(game->renderer, 0xaa, 0xaa, 0xaa, 0xff);
        SDL_RenderFillRect(game->renderer, &rect);
    }

    freeGrid(grid);

    SDL_RenderPresent(game->renderer);

    return;

}

void loadExample(game_t* game, example_t example) {

    int cols = game->grid.cols;
    int rows = game->grid.rows;

    pthread_mutex_lock(&game->gridLock);
    memset(game->grid.cells, false, cols * rows * sizeof(bool));

    int startingX = 0;
    int startingY = 0;

    switch (example.hpos) {
        case LEFT: startingX = 0; break;
        case CENTER: startingX = cols / 2 - example.cols / 2; break;
        case RIGHT: startingX = cols - example.cols; break;
    }

    switch (example.vpos) {
        case TOP: startingY = 0; break;
        case MIDDLE: startingY = rows / 2 - example.rows / 2; break;
        case BOTTOM: startingY = rows - example.rows; break;
    }

    for (int y = 0; y < example.rows; y++)
    for (int x = 0; x < example.cols; x++) {
        if ( example.cells[y * example.cols + x] )
            game->grid.cells[(startingY + y) * cols + startingX + x] = true;
    }

    pthread_mutex_unlock(&game->gridLock);

    return;

}



