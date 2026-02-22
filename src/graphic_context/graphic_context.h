#ifndef GRAPHIC_CONTEXT_H
#define GRAPHIC_CONTEXT_H

typedef struct graphic_context_t graphic_context_t;

#include <SDL3/SDL.h>
#include "../types/direction.h"
#include "../parameters/parameters.h"
#include "../render_state/render_state.h"

graphic_context_t* createGraphicContext();
void setGraphicContextParameters(graphic_context_t* gc, parameters_t params);
void draw(graphic_context_t* gc, render_state_t* rs);
Uint32 getFPSTargetTicks(graphic_context_t* gc);
void processShiftGraphicContext(graphic_context_t* gc, direction_t direction);
SDL_Point getCellOnThisPixel(graphic_context_t* gc, Sint32 x, Sint32 y);
void destroyGraphicContext(graphic_context_t** gc);

#endif
