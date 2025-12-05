#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "utils.h"
#include "config.h"
#include "grid.h"
#include "direction.h"
#include "example.h"
#include "graphic_context.h"

typedef struct {
    Sint32 mouseX;
    Sint32 mouseY;

    pthread_t updaterThread;

    grid_t grid;
    pthread_mutex_t gridLock;
    int delay;
    bool updating;
    bool running;
} game_t;

game_t* createGame();
void setGameConfig(game_t* game, config_t config);
void beginUpdating(game_t* game);
void figureSensibleDefaults(game_t* game, graphic_context_t* gc);
void update(game_t* game);
void draw(game_t* game, graphic_context_t* gc);
void handleEvents(game_t* game, graphic_context_t* gc, SDL_Event event);
void loadExample(game_t* game, example_t example);
void destroyGame(game_t* game);

#endif
