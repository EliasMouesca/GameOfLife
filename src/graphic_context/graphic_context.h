#ifndef GRAPHIC_CONTEXT_H
#define GRAPHIC_CONTEXT_H

#include <SDL3/SDL.h>

#include "../utils/utils.h"
#include "../config/config.h"
#include "../log/log.h"
#include "../parameters/parameters.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int blockSize;
    int fps;

} graphic_context_t;

graphic_context_t* createGraphicContext();
void setGraphicContextParameters(graphic_context_t* gc, parameters_t params);
void destroyGraphicContext(graphic_context_t** gc);

#endif
