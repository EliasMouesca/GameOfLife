#ifndef GRAPHIC_CONTEXT_H
#define GRAPHIC_CONTEXT_H

#include <SDL3/SDL.h>

#include "../utils/utils.h"
#include "../config/config.h"
#include "../log/log.h"
#include "../parameters/parameters.h"
#include "../grid/grid.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    grid_t grid;
    int blockSize;
    int fps;
    bool party;

} graphic_context_t;

typedef struct {
    SDL_Color color;
    bool alive;
} drawable_cell_t;

graphic_context_t* createGraphicContext();
void setGraphicContextParameters(graphic_context_t* gc, parameters_t params);
void destroyGraphicContext(graphic_context_t** gc);

#endif
