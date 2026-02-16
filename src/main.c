#include <stdio.h>
#include <string.h>
#include <stdbool.h>

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

    // TODO: Obtener config, default y opts parameters
    config_t config;

    parameters_t defaultParameters = getDefaultParameters();
    parameters_t configParameters = configToParameters(&config);
    parameters_t optionsParameters = optionsToParameters();
    
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


// Some configurations come from files, others from sensible defaults and others from cli arguments
parameters_t getParameters() {
    // TODO: Solve later
    /*
    config_t config;
    bool useConfig = true;

    char configPath[CONFIG_PATH_MAX_SIZE] = {0};

    if (argc >= 2) {
        size_t size = strlen(argv[1]);
        if (size < CONFIG_PATH_MAX_SIZE)
            strncpy(configPath, argv[1], CONFIG_PATH_MAX_SIZE - 1);
        else
            die("Config path '%s' too large (max: %d characters)", argv[1], CONFIG_PATH_MAX_SIZE);
    } else useConfig = false;

    if (useConfig) {
        if (parseConfig(configPath, &config) != 0) die("Failed to open %s", configPath);
        //setGameConfig(game, config);
        debug("Loaded game config '%s'", configPath);
        setGraphicContextConfig(gc, config);
        debug("Loaded graphic context config '%s'", configPath);

    } else {
        info("No config file found, figuring out defaults");
        figureSensibleDefaults(game, gc);
    }
    */

    return getDefaultParameters();

}
