#ifndef GAME_H
#define GAME_H

#include <SDL3/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <time.h>
#include <math.h>

#include "../utils/utils.h"
#include "../config/config.h"
#include "../grid/grid.h"
#include "../types/direction.h"
#include "../example/example.h"
#include "../graphic_context/graphic_context.h"
#include "../log/log.h"

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
void setGameParameters(game_t* game, parameters_t params);
void beginUpdating(game_t* game);
void update(game_t* game);
void draw(game_t* game, graphic_context_t* gc);
void handleEvents(game_t* game, graphic_context_t* gc, SDL_Event event);
void loadExample(game_t* game, example_t example);
void destroyGame(game_t** game);

#endif
