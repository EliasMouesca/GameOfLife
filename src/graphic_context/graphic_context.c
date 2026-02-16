
#include "graphic_context.h"

#define SDL_INIT_FLAGS SDL_INIT_VIDEO
#define WINDOW_TITLE "Game of Life"

#define TEMPORARY_WIDTH 800
#define TEMPORARY_HEIGHT 800

graphic_context_t* createGraphicContext() {
    graphic_context_t* gc = (graphic_context_t*) malloc(sizeof(graphic_context_t));
    if (!gc) die("Not enough memory for graphic context allocation");

    if (SDL_Init(SDL_INIT_FLAGS) != 0)
        die("Could not initialize SDL: %s", SDL_GetError());

    const int windowFlags = SDL_WINDOW_HIDDEN;
    gc->window = SDL_CreateWindow(WINDOW_TITLE,
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        TEMPORARY_WIDTH, TEMPORARY_HEIGHT,      // Will be resized later
        windowFlags);
    if (gc->window == NULL)
        die("Could not create window: %s", SDL_GetError());

    gc->renderer = SDL_CreateRenderer(gc->window, -1, 0);
    if (gc->renderer == NULL)
        die("Could not create renderer: %s", SDL_GetError());


    gc->blockSize = 0;
    gc->fps = 0;


    return gc;
}

void setGraphicContextParameters(graphic_context_t* gc, parameters_t params) {
    int windowWidth = params.cols * params.blockSize;
    int windowHeight = params.rows * params.blockSize;

    gc->blockSize = params.blockSize;
    gc->fps = params.fps;

    SDL_SetWindowSize(gc->window, windowWidth, windowHeight);
    SDL_ShowWindow(gc->window);

    return;
}

void destroyGraphicContext(graphic_context_t** gcp) {
    graphic_context_t* gc = *gcp;
    if (!gc) return;

    if (gc->renderer) {
        SDL_DestroyRenderer(gc->renderer);
        gc->renderer = NULL;
    }

    if (gc->window) {
        SDL_DestroyWindow(gc->window);
        gc->window = NULL;
    }

    free(gc);
    *gcp = NULL;

}


