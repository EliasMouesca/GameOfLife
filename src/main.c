#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <SDL3/SDL_main.h>

#include "utils/utils.h"
#include "game/game.h"
#include "grid/grid.h"
#include "config/config.h"
#include "example/example.h"
#include "log/log.h"
#include "parameters/parameters.h"
#include "options/options.h"

#define CONFIG_PATH_MAX_SIZE 256

int main(int argc, char* argv[]) {
    info("Logging is active");
        
    game_t* game = createGame();
    debug("Game created");
    graphic_context_t* gc = createGraphicContext();
    debug("Graphic context created");

    config_t* config = createConfig();
    debug("Config created");
    chooseAndOpenConfig(argc, argv, config);

    SDL_DisplayID display = SDL_GetPrimaryDisplay();
    int screenWidth = 0, screenHeight = 0;
    const SDL_DisplayMode* mode = SDL_GetCurrentDisplayMode(display);
    if (!mode) {
        error("Error getting screen size: %s", SDL_GetError());
    } else {
        screenWidth  = mode->w;
        screenHeight = mode->h;
    }


    parameters_t defaultParameters = getSensibleDefaultParameters(screenWidth, screenHeight);
    parameters_t configParameters = configToParameters(config);
    destroyConfig(&config);
    parameters_t optionsParameters = optionsToParameters(argc, argv);
    
    parameters_t params = solveParameters(defaultParameters, configParameters, optionsParameters);


    setGameParameters(game, params);
    setGraphicContextParameters(gc, params);

    example_t e = chaos();
    loadExample(game, e);
    destroyExample(&e);

    beginUpdating(game);

    while (game->running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) handleEvents(game, gc, event);
 
        draw(game, gc);

        SDL_Delay(1000 / gc->fps);
    }

    destroyGame(game);

    return 0;
}

