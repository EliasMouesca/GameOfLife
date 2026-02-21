#ifndef RENDER_STATE_H
#define RENDER_STATE_H

#include <SDL3/SDL.h>

#include "../grid/grid.h"

typedef struct game_t game_t;

typedef struct {
    Sint32 mouseX;
    Sint32 mouseY;

    grid_t* grid;

} render_state_t;

render_state_t* createRenderState(grid_t* grid, Sint32 mouseX, Sint32 mouseY);
void destroyRenderState(render_state_t** renderState);

#endif
