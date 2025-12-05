#ifndef GRAPHIC_CONTEXT_H
#define GRAPHIC_CONTEXT_H

#include <SDL2/SDL.h>

#include "utils.h"
#include "config.h"

typedef struct {
    SDL_Window* window;
    SDL_Renderer* renderer;
    int blockSize;
    int fps;

} graphic_context_t;

graphic_context_t* createGraphicContext();
void setGraphicContextParams(graphic_context_t* gc, int windowWidth, int windowHeight, int windowFlags, int blockSize, int fps);
void setGraphicContextConfig(graphic_context_t* gc, config_t config);
void destroyGraphicContext(graphic_context_t** gc);

#endif
