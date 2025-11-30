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

    game.running = true;
    while (game.running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) {
            handleEvents(&game, event);
        }
 
        draw(&game);

        SDL_Delay(1000 / game.fps);
    }

    free(game.grid.cells);

    return 0;
}
