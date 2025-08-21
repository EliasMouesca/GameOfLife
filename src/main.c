#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include "utils.h"
#include "game.h"
#include "grid.h"
#include "config.h"

pthread_mutex_t gridLock;
grid_t grid;

void* gameUpdater(void* arg) {
    Uint32 delay = *((Uint32*) arg);

    while (true) {
        pthread_mutex_lock(&gridLock);
        update(grid);
        pthread_mutex_unlock(&gridLock);
        SDL_Delay(delay);
    }

    return NULL;
}

int main(void) {
    config_t config;

    char configPath[] = "config.txt";
    if (parseConfig(configPath, &config) != 0)
        die("Failed to open %s", configPath);

    const char* rowsStr = getValue(&config, "rows");
    const char* colsStr = getValue(&config, "cols");
    const char* blockSizeStr = getValue(&config, "block_size");
    const char* fpsStr = getValue(&config, "fps");
    const char* delayStr = getValue(&config, "delay");

    grid.rows = atoi(rowsStr);
    grid.cols = atoi(colsStr);
    int blockSize = atoi(blockSizeStr);
    int fps = atoi(fpsStr);
    int delay = atoi(delayStr);

    grid.cells = malloc(grid.rows * grid.cols * sizeof(bool));

    memset(grid.cells, 0, grid.rows * grid.cols * sizeof(bool));

    int middleX = grid.cols / 2;
    int middleY = grid.rows / 2;

    grid.cells[(middleY - 1) * grid.cols + (middleX - 1)] = true;
    grid.cells[(middleY - 1) * grid.cols + middleX] = true;
    grid.cells[middleY * grid.cols + middleX] = true;
    grid.cells[middleY * grid.cols + (middleX + 1)] = true;
    grid.cells[(middleY + 1) * grid.cols + middleX] = true;


    int windowWidth = grid.cols * blockSize;
    int windowHeight = grid.cols * blockSize;


    Uint32 initFlags = SDL_INIT_VIDEO;

    if (SDL_Init(initFlags) != 0)
        die("Could not initialize SDL: %s", SDL_GetError());

    SDL_Window* window = SDL_CreateWindow("DEMO",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            windowWidth, windowHeight,
            0);
    if (window == NULL)
        die("Could not create window: %s", SDL_GetError());

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, 0);
    if (renderer == NULL)
        die("Could not create renderer: %s", SDL_GetError());

    pthread_mutex_init(&gridLock, NULL);

    pthread_t thread;
    pthread_create(&thread, NULL, gameUpdater, &delay);

    bool running = true;
    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) running = false;
        }

        draw(renderer, grid, blockSize);

        SDL_Delay(1000 / fps);
    }

    free(grid.cells);

    return 0;
}
