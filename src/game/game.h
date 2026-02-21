#ifndef GAME_H
#define GAME_H

#include "../parameters/parameters.h"
#include "../render_state/render_state.h"

#include <SDL3/SDL.h>

typedef struct game_t game_t;

typedef struct grid_t grid_t;
typedef struct graphic_context_t graphic_context_t;
typedef struct example_t example_t;

game_t* createGame();
void setGameParameters(game_t* game, parameters_t params);
void beginUpdating(game_t* game);
void update(game_t* game);
render_state_t* createRenderStateFromGame(game_t* game);
bool runGame(game_t* game, graphic_context_t* gc);
void handleEvents(game_t* game, graphic_context_t* gc, SDL_Event event);
void loadExample(game_t* game, example_t* example);
void destroyGame(game_t** game);

#endif
