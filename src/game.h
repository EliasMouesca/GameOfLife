#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "grid.h"

typedef enum {
    DIRECTION_UP, 
    DIRECTION_LEFT, 
    DIRECTION_DOWN, 
    DIRECTION_RIGHT,
    DIRECTION_NONE
} direction_t;

void update(grid_t grid);
void draw(SDL_Renderer* renderer, grid_t grid, int blockSize, Sint32 mouseX, Sint32 mouseY);
void shiftGrid(grid_t grid, direction_t direction);

#endif
