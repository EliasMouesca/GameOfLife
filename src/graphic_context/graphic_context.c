#include "graphic_context.h"

#include <SDL3/SDL.h>
#include <SDL3_ttf/SDL_ttf.h>
#include <math.h>

#include "../utils/utils.h"
#include "../config/config.h"
#include "../log/log.h"
#include "../parameters/parameters.h"
#include "../render_state/render_state.h"
#include "../grid/grid.h"
#include "../types/direction.h"

#define SDL_INIT_FLAGS SDL_INIT_VIDEO
#define WINDOW_TITLE "Game of Life"

#define TEMPORARY_WIDTH 800
#define TEMPORARY_HEIGHT 800

#define COLOR_SUPERIOR_LIMIT 255
#define COLOR_INFERIOR_LIMIT 100

// -- Structs --
struct graphic_context_t {
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    TTF_Font* font;

    grid_t* grid;
    int blockSize;
    int fps;
    bool party;

    Uint64 fpsCounter;
    double fpsSmooth;
};

typedef struct drawable_cell_t {
    SDL_Color color;
    bool alive;
} drawable_cell_t;
// ----------------

// ---- Forward declarations ----
SDL_Color getNextRandomColor();
SDL_Color getTrueRandomColor();
void setDrawColor(SDL_Renderer* ren, SDL_Color color);
void drawFPSBox(SDL_Renderer* ren, TTF_Font* font, Uint64 fps);
// ------------------------------

SDL_Point getCellOnThisPixel(graphic_context_t* gc, Sint32 x, Sint32 y) {
    int c = x / gc->blockSize;
    int r = y / gc->blockSize;

    return (SDL_Point){.x=c, .y=r};
}


void processShiftGraphicContext(graphic_context_t* gc, direction_t direction) {
    shiftGrid(gc->grid, direction);
    return;
}

Uint32 getFPSTargetTicks(graphic_context_t* gc) {
    const Uint64 freq = SDL_GetPerformanceFrequency();
    return (gc->fps > 0) ? (freq / (Uint64)gc->fps) : 0;
}

graphic_context_t* createGraphicContext() {
    graphic_context_t* gc = (graphic_context_t*) malloc(sizeof(graphic_context_t));
    if (!gc) critical("Not enough memory for graphic context allocation");

    if (!SDL_Init(SDL_INIT_FLAGS))
        critical("Could not initialize SDL: %s", SDL_GetError());

    if (!TTF_Init())
        critical("Could not initialize TTF library: %s", SDL_GetError());

    gc->font = TTF_OpenFont("./fonts/SpaceMono-Regular.ttf", 14);
    if (!gc->font)
        critical("Could not open font");

    const int windowFlags = SDL_WINDOW_HIDDEN;
    gc->window = SDL_CreateWindow(WINDOW_TITLE,
        TEMPORARY_WIDTH, TEMPORARY_HEIGHT,      // Will be resized later
        windowFlags);
    if (gc->window == NULL)
        critical("Could not create window: %s", SDL_GetError());

    gc->renderer = SDL_CreateRenderer(gc->window, NULL);
    if (gc->renderer == NULL)
        critical("Could not create renderer: %s", SDL_GetError());

    gc->grid = makeEmptyGrid();

    gc->blockSize = 0;
    gc->fps = 0;

    gc->fpsCounter = SDL_GetPerformanceCounter();
    gc->fpsSmooth = 0.0;

    return gc;
}

void setGraphicContextParameters(graphic_context_t* gc, parameters_t params) {
    char* buffer = malloc(256 * sizeof(char));
    if (!areAllParametersSet(params, buffer))
        critical("Called setGraphicsContextParameters with '%s' not set", buffer);
    free(buffer);

    drawable_cell_t model = {.color={.r=0,.g=0,.b=0,.a=0},.alive=false};
    freeGrid(&gc->grid);
    gc->grid = makeModelGrid(params.rows, params.cols, sizeof(drawable_cell_t), &model);

    gc->blockSize = params.blockSize;
    gc->fps = params.fps;
    gc->party = params.party;

    if (params.fullscreen) {
        SDL_SetWindowFullscreen(gc->window, true);
    } else {
        int windowWidth = params.cols * params.blockSize;
        int windowHeight = params.rows * params.blockSize;
        SDL_SetWindowSize(gc->window, windowWidth, windowHeight);
    }

    SDL_ShowWindow(gc->window);

    return;
}

void draw(graphic_context_t* gc, render_state_t* rs) {
    int rows = rs->grid->rows;
    int cols = rs->grid->cols;
    bool* gameCells = (bool*) rs->grid->cells;
    drawable_cell_t* gcCells = (drawable_cell_t*) gc->grid->cells;

    Sint32 mouseX = rs->mouseX;
    Sint32 mouseY = rs->mouseY;

    int blockSize = gc->blockSize;
    SDL_SetRenderDrawColor(gc->renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(gc->renderer);

    for (int y = 0; y < rows; y++)
    for (int x = 0; x < cols; x++) {
        int index = y * cols + x;
        if (gameCells[index]) {
            if (gc->party) {
                if (!gcCells[index].alive){
                    SDL_Color newColor = getNextRandomColor();
                    gcCells[index].color = newColor;
                }
                    setDrawColor(gc->renderer, gcCells[index].color);

            } else SDL_SetRenderDrawColor(gc->renderer, 0xff, 0xff, 0xff, 0xff);

            gcCells[index].alive = true;

        } else {
            gcCells[index].alive = false;
            SDL_SetRenderDrawColor(gc->renderer, 0x0, 0x0, 0x0, 0x0);
        }
        SDL_FRect rect = {(float)(x * blockSize), (float)(y * blockSize), (float)(blockSize), (float)(blockSize)};
        SDL_RenderFillRect(gc->renderer, &rect);

    }

    if ( (mouseX > 0 && mouseX < cols * blockSize) && 
            (mouseY > 0 && mouseY < rows * blockSize) )
    {
        int x = mouseX - (mouseX % blockSize);
        int y = mouseY - (mouseY % blockSize);

        SDL_FRect rect = {(float)x, (float)y, (float)blockSize, (float)blockSize};
        SDL_SetRenderDrawColor(gc->renderer, 0xaa, 0xaa, 0xaa, 0xff);
        SDL_RenderFillRect(gc->renderer, &rect);
    }

    bool countFps = true;
    Uint64 now = SDL_GetPerformanceCounter();
    Uint64 freq = SDL_GetPerformanceFrequency();

    double dt = (double)(now - gc->fpsCounter) / (double)freq;

    if (dt > 0.0) {
        double fps = 1.0 / dt;

        if (gc->fpsSmooth == 0.0)
            gc->fpsSmooth = fps;
        else
            gc->fpsSmooth = gc->fpsSmooth * 0.90 + fps * 0.10;

        drawFPSBox(gc->renderer, gc->font, (Uint64)(gc->fpsSmooth + 0.5));
    }

    gc->fpsCounter = now;

    SDL_RenderPresent(gc->renderer);

    return;
}

void drawFPSBox(SDL_Renderer* ren, TTF_Font* font, Uint64 fps) {
    static SDL_Texture* text_tex = NULL;
    static int tex_w = 0;
    static int tex_h = 0;
    static Uint64 last_fps = (Uint64)-1;

    char buffer[32];

    // recrear textura solo si cambia FPS
    if (fps != last_fps) {
        last_fps = fps;

        if (text_tex) {
            SDL_DestroyTexture(text_tex);
            text_tex = NULL;
        }

        snprintf(buffer, sizeof(buffer), "FPS: %llu", (unsigned long long)fps);

        SDL_Color color = {255, 255, 255, 255};
        SDL_Surface* surf = TTF_RenderText_Blended(font, buffer, 0, color);
        if (!surf) return;

        text_tex = SDL_CreateTextureFromSurface(ren, surf);
        tex_w = surf->w;
        tex_h = surf->h;

        SDL_DestroySurface(surf);
    }

    if (!text_tex) return;

    const int padding = 6;

    SDL_FRect bg = {
        10,
        10,
        tex_w + padding * 2,
        tex_h + padding * 2
    };

    // fondo semitransparente
    SDL_SetRenderDrawBlendMode(ren, SDL_BLENDMODE_BLEND);
    SDL_SetRenderDrawColor(ren, 0, 0, 0, 160);
    SDL_RenderFillRect(ren, &bg);

    SDL_FRect dst = {
        bg.x + padding,
        bg.y + padding,
        tex_w,
        tex_h
    };

    SDL_RenderTexture(ren, text_tex, NULL, &dst);
}

SDL_Color getNextRandomColor() {
    static Uint8 red = 0xff;
    static Uint8 green = 0xff;
    static Uint8 blue = 0xff;
    static bool init = true;

    if (init) {
        init = false;
        SDL_Color base = getTrueRandomColor();

        red = base.r;
        green = base.g;
        blue = base.b;
    }


    Uint8* v = NULL;
    switch (rand() % 3) {
        case 0:
            v = &red;
            break;
        case 1:
            v = &green;
            break;
        case 2:
            v = &blue;
            break;
    }

    const int sup = COLOR_SUPERIOR_LIMIT;
    const int inf = COLOR_INFERIOR_LIMIT;
    const int definition = 10000;
    //const int diff = 256 / 30;
    const int diff = 5;

    // --- "pull to center": fuerte en extremos, tranqui en el medio ---
    // Probabilidad p in [0,1] de moverse hacia el centro:
    //   p(mid)=0, p(inf)=1, p(sup)=1
    // gamma controla cuán "fuerte" son los extremos (>=2 recomendado)
    const double gamma = 10.;

    const double mid  = 0.5 * (inf + sup);
    const double half = 0.5 * (sup - inf);

    double d = fabs((double)(*v) - mid) / half;   // 0..1
    if (d < 0.0) d = 0.0;
    if (d > 1.0) d = 1.0;

    double p = pow(d, gamma);
    int y = (int)((double)definition * p);

    int delta;
    if (rand() % definition < y) {
        delta = (*v > (Uint8)mid) ? -diff : +diff;
    } else {
        delta = (rand() & 1) ? +diff : -diff;
    }

    int next = (int)(*v) + delta;
    if (next < inf) next = inf;
    if (next > sup) next = sup;
    *v = (Uint8)next;

    return (SDL_Color){.r=red, .g=green, .b=blue, .a=255};
}

SDL_Color getTrueRandomColor() {
    // True random
    const int inf = COLOR_INFERIOR_LIMIT;
    const int sup = COLOR_SUPERIOR_LIMIT;

    Uint8 r = rand() % (sup - inf) + inf;
    Uint8 g = rand() % (sup - inf) + inf;
    Uint8 b = rand() % (sup - inf) + inf;

    return (SDL_Color){.r=r, .g=g, .b=b, .a=255};
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

    freeGrid(&gc->grid);

    free(gc);
    *gcp = NULL;

    SDL_Quit();
}


