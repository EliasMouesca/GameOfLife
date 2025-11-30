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
#define SDL_INIT_FLAGS SDL_INIT_VIDEO

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

    game_t* game = createGame();
    setGameConfig(game, config);

    if (SDL_Init(SDL_INIT_FLAGS) != 0)
        die("Could not initialize SDL: %s", SDL_GetError());

    int windowWidth = game->grid.cols * game->blockSize;
    int windowHeight = game->grid.rows * game->blockSize;

    game->window = SDL_CreateWindow("DEMO",
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            windowWidth, windowHeight,
            0);
    if (game->window == NULL)
        die("Could not create window: %s", SDL_GetError());

    game->renderer = SDL_CreateRenderer(game->window, -1, 0);
    if (game->renderer == NULL)
        die("Could not create renderer: %s", SDL_GetError());

    pthread_mutex_init(&game->gridLock, NULL);

    game->running = true;
    while (game->running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) handleEvents(game, event);
 
        draw(game);

        SDL_Delay(1000 / game->fps);
    }

    destroyGame(game);

    return 0;
}
