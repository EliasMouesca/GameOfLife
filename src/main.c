#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include "utils.h"
#include "game.h"
#include "grid.h"
#include "config.h"
#include "example.h"

#define CONFIG_PATH_MAX_SIZE 256

game_t game;

void* gameUpdater(void* arg) {
    //Uint32 delay = *((Uint32*) arg);

    while (game.updating && game.running) {
        update(&game);
        SDL_Delay(game.delay);
    }

    return NULL;
}

int main(int argc, char* argv[]) {
    config_t config;

    char configPath[CONFIG_PATH_MAX_SIZE];

    if (argc >= 2) {
        size_t size = strlen(argv[1]);
        if (size < CONFIG_PATH_MAX_SIZE)
            strcpy(configPath, argv[1]);
        else
            die("Config path '%s' too large (max: %d characters)", argv[1], CONFIG_PATH_MAX_SIZE);
    } else
        strcpy(configPath, "config.txt");
        
    if (parseConfig(configPath, &config) != 0)
        die("Failed to open %s", configPath);

    const char* rowsStr = getValue(&config, "rows");
    const char* colsStr = getValue(&config, "cols");
    const char* blockSizeStr = getValue(&config, "block_size");
    const char* fpsStr = getValue(&config, "fps");
    const char* delayStr = getValue(&config, "delay");

    if (!rowsStr) die("Could not parse 'rows' config parameter");
    int rows = (game.grid.rows = atoi(rowsStr));
    if (!colsStr) die("Could not parse 'cols' config parameter");
    int cols = (game.grid.cols = atoi(colsStr));
    if (!blockSizeStr) die("Could not parse 'blockSize' config parameter");
    game.blockSize = atoi(blockSizeStr);
    if (!fpsStr) die("Could not parse 'fps' config parameter");
    game.fps = atoi(fpsStr);
    if (!delayStr) die("Could not parse 'delay' config parameter");
    game.delay = atoi(delayStr);

    void* cells = game.grid.cells = malloc(rows * cols * sizeof(bool));

    memset(cells, 0, rows * cols * sizeof(bool));

    example_t e = chaos();
    loadExample(&game, e);
    destroyExample(&e);

    int windowWidth = cols * game.blockSize;
    int windowHeight = rows * game.blockSize;

    Uint32 initFlags = SDL_INIT_VIDEO;

    if (SDL_Init(initFlags) != 0)
        die("Could not initialize SDL: %s", SDL_GetError());

    game.window = SDL_CreateWindow("DEMO",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            windowWidth, windowHeight,
            0);
    if (game.window == NULL)
        die("Could not create window: %s", SDL_GetError());

    game.renderer = SDL_CreateRenderer(game.window, -1, 0);
    if (game.renderer == NULL)
        die("Could not create renderer: %s", SDL_GetError());

    pthread_mutex_init(&game.gridLock, NULL);

    pthread_t thread;

    game.running = true;
    Sint32 mouseX = -1, mouseY = -1;
    while (game.running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            switch (event.type) {
                case SDL_QUIT:
                    game.running = false;
                    break;

                case SDL_MOUSEMOTION:
                    mouseX = event.motion.x;
                    mouseY = event.motion.y;
                    break;

                case SDL_MOUSEBUTTONDOWN:
                    int c = event.button.x / game.blockSize;
                    int r = event.button.y / game.blockSize;

                    pthread_mutex_lock(&game.gridLock);
                    game.grid.cells[r * game.grid.cols + c] = !game.grid.cells[r * game.grid.cols + c];
                    pthread_mutex_unlock(&game.gridLock);

                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym) {
                        case SDLK_SPACE:
                            game.updating = !game.updating;
                            if (game.updating)
                                pthread_create(&thread, NULL, gameUpdater, NULL);
                            break;
                        case SDLK_n:
                            update(&game);
                            break;
                        case SDLK_0:
                            e = nothing();
                            pthread_mutex_lock(&game.gridLock);
                            loadExample(&game, e);
                            pthread_mutex_unlock(&game.gridLock);
                            destroyExample(&e);
                            break;
                        case SDLK_1:
                            e = chaos();
                            pthread_mutex_lock(&game.gridLock);
                            loadExample(&game, e);
                            pthread_mutex_unlock(&game.gridLock);
                            destroyExample(&e);
                            break;
                        case SDLK_2:
                            e = diehard();
                            pthread_mutex_lock(&game.gridLock);
                            loadExample(&game, e);
                            pthread_mutex_unlock(&game.gridLock);
                            destroyExample(&e);
                            break;
                        case SDLK_3:
                            e = glider();
                            pthread_mutex_lock(&game.gridLock);
                            loadExample(&game, e);
                            pthread_mutex_unlock(&game.gridLock);
                            destroyExample(&e);
                            break;
                        case SDLK_4:
                            e = acorn();
                            pthread_mutex_lock(&game.gridLock);
                            loadExample(&game, e);
                            pthread_mutex_unlock(&game.gridLock);
                            destroyExample(&e);
                            break;
                        case SDLK_5:
                            e = galaxy();
                            pthread_mutex_lock(&game.gridLock);
                            loadExample(&game, e);
                            pthread_mutex_unlock(&game.gridLock);
                            destroyExample(&e);
                            break;

                        case SDLK_UP:
                        case SDLK_LEFT:
                        case SDLK_DOWN:
                        case SDLK_RIGHT:
                            pthread_mutex_lock(&game.gridLock);
                            direction_t direction;
                            switch (event.key.keysym.sym) {
                                case SDLK_UP: direction = DIRECTION_UP; break;
                                case SDLK_LEFT: direction = DIRECTION_LEFT; break;
                                case SDLK_DOWN: direction = DIRECTION_DOWN; break;
                                case SDLK_RIGHT: direction = DIRECTION_RIGHT; break;
                            }
                            shiftGrid(game.grid, direction);
                            pthread_mutex_unlock(&game.gridLock);
                            break;

                    } // END switch(event.key.keycode)
                    break;
                } // END switch(event.type)
        } // END while(SDL_Poll)
 
        draw(&game, mouseX, mouseY);

        SDL_Delay(1000 / game.fps);
    } // END while(game.running)

    free(game.grid.cells);

    return 0;
}
