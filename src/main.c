#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include "utils.h"
#include "game.h"
#include "grid.h"
#include "config.h"
#include "examples.h"

pthread_mutex_t gridLock;
grid_t grid;

bool updating = false;

void* gameUpdater(void* arg) {
    Uint32 delay = *((Uint32*) arg);

    while (updating) {
        pthread_mutex_lock(&gridLock);
        update(grid);
        pthread_mutex_unlock(&gridLock);
        SDL_Delay(delay);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    config_t config;

    char configPath[64];

    if (argc >= 2) 
        strcpy(configPath, argv[1]);
    else
        strcpy(configPath, "config.txt");
        
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

    example_t e = chaos();
    loadExample(grid, e);
    destroyExample(&e);

    int windowWidth = grid.cols * blockSize;
    int windowHeight = grid.rows * blockSize;

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

    bool running = true;
    Sint32 mouseX = -1, mouseY = -1;
    while (running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    running = false;
                    break;

                case SDL_MOUSEMOTION:
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    int c = event.button.x / blockSize;
                    int r = event.button.y / blockSize;

                    pthread_mutex_lock(&gridLock);
                    grid.cells[r * grid.cols + c] = !grid.cells[r * grid.cols + c];
                    pthread_mutex_unlock(&gridLock);

                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_SPACE:
                            updating = !updating;
                            if (updating)
                                pthread_create(&thread, NULL, gameUpdater, &delay);
                            break;
                        case SDLK_n:
                            pthread_mutex_lock(&gridLock);
                            update(grid);
                            pthread_mutex_unlock(&gridLock);
                            break;
                        case SDLK_0:
                            e = nothing();
                            pthread_mutex_lock(&gridLock);
                            loadExample(grid, e);
                            pthread_mutex_unlock(&gridLock);
                            destroyExample(&e);
                            break;
                        case SDLK_1:
                            e = chaos();
                            pthread_mutex_lock(&gridLock);
                            loadExample(grid, e);
                            pthread_mutex_unlock(&gridLock);
                            destroyExample(&e);
                            break;
                        case SDLK_2:
                            e = diehard();
                            pthread_mutex_lock(&gridLock);
                            loadExample(grid, e);
                            pthread_mutex_unlock(&gridLock);
                            destroyExample(&e);
                            break;
                        case SDLK_3:
                            e = glider();
                            pthread_mutex_lock(&gridLock);
                            loadExample(grid, e);
                            pthread_mutex_unlock(&gridLock);
                            destroyExample(&e);
                            break;
                        case SDLK_4:
                            e = acorn();
                            pthread_mutex_lock(&gridLock);
                            loadExample(grid, e);
                            pthread_mutex_unlock(&gridLock);
                            destroyExample(&e);
                            break;
                        case SDLK_5:
                            e = galaxy();
                            pthread_mutex_lock(&gridLock);
                            loadExample(grid, e);
                            pthread_mutex_unlock(&gridLock);
                            destroyExample(&e);
                            break;

                        case SDLK_UP:
                        case SDLK_LEFT:
                        case SDLK_DOWN:
                        case SDLK_RIGHT:
                            pthread_mutex_lock(&gridLock);
                            direction_t direction;
                            switch (event.key.keysym.sym) {
                                case SDLK_UP: direction = DIRECTION_UP; break;
                                case SDLK_LEFT: direction = DIRECTION_LEFT; break;
                                case SDLK_DOWN: direction = DIRECTION_DOWN; break;
                                case SDLK_RIGHT: direction = DIRECTION_RIGHT; break;
                            }
                            shiftGrid(grid, direction);
                            pthread_mutex_unlock(&gridLock);
                            break;

                    } // END switch(event.key.keycode)
                    break;
                } // END switch(event.type)
        } // END while(SDL_Poll)
 
        pthread_mutex_lock(&gridLock);
        draw(renderer, grid, blockSize, mouseX, mouseY);
        pthread_mutex_unlock(&gridLock);

        SDL_Delay(1000 / fps);
    } // END while(running)

    free(grid.cells);

    return 0;
}
