#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#include "utils.h"
#include "game.h"
#include "grid.h"
#include "config.h"
#include "example.h"
#include "log.h"

#define CONFIG_PATH_MAX_SIZE 256

int main(int argc, char* argv[]) {
    info("Logging is active");
    config_t config;

    char configPath[CONFIG_PATH_MAX_SIZE] = {0};

    if (argc >= 2) {
        size_t size = strlen(argv[1]);
        if (size < CONFIG_PATH_MAX_SIZE)
            strncpy(configPath, argv[1], CONFIG_PATH_MAX_SIZE - 1);
        else
            die("Config path '%s' too large (max: %d characters)", argv[1], CONFIG_PATH_MAX_SIZE);
    } else
        strncpy(configPath, "config.txt", CONFIG_PATH_MAX_SIZE - 1);
        
    if (parseConfig(configPath, &config) != 0)
        die("Failed to open %s", configPath);

    game_t* game = createGame();
    info("Game created");

    setGameConfig(game, config);
    info("Loaded game config '%s'", configPath);

    graphic_context_t* gc = createGraphicContext(config);
    info("Graphic context created");

    while (game->running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) handleEvents(game, gc, event);
 
        draw(game, gc);

        SDL_Delay(1000 / gc->fps);
    }

    destroyGame(game);

    return 0;
}
