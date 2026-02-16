
#include "graphic_context.h"

#define SDL_INIT_FLAGS SDL_INIT_VIDEO
#define WINDOW_TITLE "Game of Life"

#define TEMPORARY_WIDTH 800
#define TEMPORARY_HEIGHT 800

graphic_context_t* createGraphicContext() {
    graphic_context_t* gc = (graphic_context_t*) malloc(sizeof(graphic_context_t));
    if (!gc) critical("Not enough memory for graphic context allocation");

    if (!SDL_Init(SDL_INIT_FLAGS))
        critical("Could not initialize SDL: %s", SDL_GetError());

    const int windowFlags = SDL_WINDOW_HIDDEN;
    gc->window = SDL_CreateWindow(WINDOW_TITLE,
        TEMPORARY_WIDTH, TEMPORARY_HEIGHT,      // Will be resized later
        windowFlags);
    if (gc->window == NULL)
        critical("Could not create window: %s", SDL_GetError());

    gc->renderer = SDL_CreateRenderer(gc->window, NULL);
    if (gc->renderer == NULL)
        critical("Could not create renderer: %s", SDL_GetError());


    gc->blockSize = 0;
    gc->fps = 0;


    return gc;
}

void setGraphicContextParameters(graphic_context_t* gc, parameters_t params) {
    if (!params.colsDefined)
        critical("Called setGraphicsContextParameters with 'cols' not set");
    if (!params.rowsDefined)
        critical("Called setGraphicsContextParameters with 'rows' not set");
    if (!params.blockSizeDefined)
        critical("Called setGraphicsContextParameters with 'blockSize' not set");
    if (!params.fpsDefined)
        critical("Called setGraphicsContextParameters with 'fps' not set");

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

    SDL_Quit();
}


