#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include <stdlib.h>
#include <string.h>

#include "utils.h"
#include "grid.h"

void update(grid_t grid);
void draw(SDL_Renderer* renderer, grid_t grid, int blockSize, Sint32 mouseX, Sint32 mouseY);

#endif
