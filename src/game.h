#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "grid.h"
#include "direction.h"
#include "example.h"

typedef struct {
    SDL_Renderer* renderer;
    SDL_Window* window;

    int blockSize;
    int fps;
    int delay;

    pthread_mutex_t gridLock;
    grid_t grid;
    bool updating;
    bool running;

} game_t;

void update(game_t* game);
void draw(game_t* game, Sint32 mouseX, Sint32 mouseY);
void loadExample(game_t* game, example_t example);

#endif
