
#include "graphic_context.h"

#define SDL_INIT_FLAGS SDL_INIT_VIDEO
#define WINDOW_TITLE "DEMO"

graphic_context_t* createGraphicContext(config_t config) {

    graphic_context_t* gc = (graphic_context_t*) malloc(sizeof(graphic_context_t));
    if (!gc) die("Not enough memory for graphic context allocation");

    if (SDL_Init(SDL_INIT_FLAGS) != 0)
        die("Could not initialize SDL: %s", SDL_GetError());

    const char* rowsStr = getValue(&config, "rows");
    const char* colsStr = getValue(&config, "cols");
    const char* blockSizeStr = getValue(&config, "block_size");
    const char* fpsStr = getValue(&config, "fps");

    if (!fpsStr|| fpsStr[0]=='\0') die("Could not parse 'rows' config parameter");
    if (!blockSizeStr|| blockSizeStr[0]=='\0') die("Could not parse 'rows' config parameter");
    if (!rowsStr || rowsStr[0]=='\0') die("Could not parse 'rows' config parameter");
    if (!colsStr || colsStr[0]=='\0') die("Could not parse 'cols' config parameter");

    int fps = atoi(blockSizeStr);
    int blockSize = atoi(blockSizeStr);
    int rows = atoi(rowsStr);
    int cols = atoi(colsStr);

    int width = cols * blockSize;
    int height = rows * blockSize;
    Uint32 flags = 0;

    gc->window = SDL_CreateWindow(WINDOW_TITLE,
            SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
            width, height,
            flags);
    if (gc->window == NULL)
        die("Could not create window: %s", SDL_GetError());

    gc->renderer = SDL_CreateRenderer(gc->window, -1, 0);
    if (gc->renderer == NULL)
        die("Could not create renderer: %s", SDL_GetError());


    gc->blockSize = blockSize;
    gc->fps = fps;

    return gc;

}

void setGraphicContextConfig(graphic_context_t* gc, config_t config) {
    const char* blockSizeStr = getValue(&config, "block_size");
    const char* fpsStr = getValue(&config, "fps");

    if (!blockSizeStr || blockSizeStr[0]=='\0') die("Could not parse 'blockSize' config parameter");
    if (!fpsStr || fpsStr[0]=='\0') die("Could not parse 'fps' config parameter");

    gc->blockSize = atoi(blockSizeStr);
    gc->fps = atoi(fpsStr);

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


