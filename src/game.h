#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "config.h"
#include "grid.h"
#include "direction.h"
#include "example.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int blockSize;
    int fps;

    Sint32 mouseX;
    Sint32 mouseY;



    grid_t grid;
    pthread_mutex_t gridLock;
    int delay;
    bool updating;
    bool running;
} game_t;

game_t* createGame();
void setGameConfig(game_t* game, config_t config);
void update(game_t* game);
void draw(game_t* game);
void handleEvents(game_t* game, SDL_Event event);
void loadExample(game_t* game, example_t example);
void destroyGame(game_t* game);

#endif
