#include "game.h"

#include <SDL3/SDL.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

#include "../utils/utils.h"
#include "../config/config.h"
#include "../types/direction.h"
#include "../example/example.h"
#include "../graphic_context/graphic_context.h"
#include "../render_state/render_state.h"
#include "../log/log.h"
#include "../grid/grid.h"
#include "../parameters/parameters.h"

#define DELAY_TINY_STEP 10
#define DELAY_BIG_STEP 25
#define DELAY_TOP_LIMIT 1000
#define DELAY_BOTTOM_LIMIT 0

struct game_t {
    Sint32 mouseX;
    Sint32 mouseY;

    pthread_t updaterThread;

    grid_t* grid;
    pthread_mutex_t gridLock;
    int delay;
    bool updating;
    bool running;

};

void* gameUpdater(void* arg);
void processShift(game_t* game, graphic_context_t* gc, direction_t direction);

game_t* createGame() {
    game_t* game = malloc(sizeof(game_t));
    game->updating = false;
    game->running = true;
    game->delay = 0;
    game->mouseX = -1;
    game->mouseY = -1;

    srand(time(NULL));

    game->grid = makeEmptyGrid();


    pthread_mutex_init(&game->gridLock, NULL);

    return game;
}

void processShift(game_t* game, graphic_context_t* gc, direction_t direction) {
    pthread_mutex_lock(&game->gridLock);
    shiftGrid(game->grid, direction);
    pthread_mutex_unlock(&game->gridLock);

    processShiftGraphicContext(gc, direction);

    return;

}

bool runGame(game_t* game, graphic_context_t* gc) {
    beginUpdating(game);

    while (game->running) {
        SDL_Event event;

        while (SDL_PollEvent(&event)) handleEvents(game, gc, event);
 
        pthread_mutex_lock(&game->gridLock);
        render_state_t* rs = createRenderStateFromGame(game);
        pthread_mutex_unlock(&game->gridLock);

        draw(gc, rs);

        destroyRenderState(&rs);

        SDL_Delay(getFPSDelay(gc));
    }

    return true;
}

render_state_t* createRenderStateFromGame(game_t* game) {
    return createRenderState(game->grid, game->mouseX, game->mouseY);
}

void beginUpdating(game_t* game) {
    if (!game) error("beginUpdating() got null argument 'game'");
    pthread_create(&game->updaterThread, NULL, gameUpdater, game);
}

void setGameParameters(game_t* game, parameters_t params) {
    if (params.delayDefined)
        game->delay = params.delay;
    else error("Called setGameParameters with 'delay' parameter not set (null)");

    if (params.rowsDefined)
        game->grid->rows = params.rows;
    else error("Called setGameParameters with 'rows' parameter not set (null)");

    if (params.colsDefined)
        game->grid->cols = params.cols;
    else error("Called setGameParameters with 'cols' parameter not set (null)");

    game->grid->cells = malloc(game->grid->rows * game->grid->cols * sizeof(bool));
    game->grid->elementSize = sizeof(bool);
    memset(game->grid->cells, 0, game->grid->rows * game->grid->cols * sizeof(bool));

    return;
}

void update(game_t* game) {
    pthread_mutex_lock(&game->gridLock);

    int cols = game->grid->cols;
    int rows = game->grid->rows;
    bool* cells = (bool*) game->grid->cells;
    bool* buffer = (bool*) malloc(rows * cols * sizeof(bool));

    for (int y = 0; y < rows; y++)
    for (int x = 0; x < cols; x++) {
        int i = y * cols + x;
        int neighborsAlive = 0;

        if ((y > 0 && x > 0) ? cells[i-cols-1] : false) neighborsAlive++;
        if ((y > 0) ? cells[i-cols] : false) neighborsAlive++;
        if ((y > 0 && x < cols - 1) ? cells[i-cols + 1] : false) neighborsAlive++;
        
        if ((x > 0) ? cells[i-1] : false) neighborsAlive++;
        if ((x < cols - 1) ? cells[i+1] : false) neighborsAlive++;

        if ((y < rows - 1 && x > 0) ? cells[i+cols-1] : false) neighborsAlive++;
        if ((y < rows - 1) ? cells[i+cols] : false) neighborsAlive++;
        if ((y < rows - 1 && x < cols - 1) ? cells[i+cols+1] : false) neighborsAlive++;

        if (cells[i]) {
            if (neighborsAlive == 2 || neighborsAlive == 3) buffer[i] = true;
            else buffer[i] = false;
        } else {
            if (neighborsAlive == 3) buffer[i] = true;
            else buffer[i] = false;
        }
    }

    memcpy(cells, buffer, rows * cols * sizeof(bool));
    pthread_mutex_unlock(&game->gridLock);

    free(buffer);

    return;
}

/*
SDL_Color getRandomColorOld(game_t* game) {
    //int i = game->random++;
    // True random
    Uint8 r = rand() % 200 + 56;
    Uint8 g = rand() % 200 + 56;
    Uint8 b = rand() % 200 + 56;
    */

    /*
    // Next random (linear)
    static Uint8 red = 0xff;
    static Uint8 green = 0xff;
    static Uint8 blue = 0xff;

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

    const double limiteInf = 55;
    const double limiteSup = 255;
    const int definicion = 1000;
    int diff = 256 / 30;

    const double m = 1 / (limiteSup - limiteInf);
    const double b = - limiteInf / (limiteSup - limiteInf);
    int y = (int) ((double) definicion * (m * *v + b));
    int lal = rand();
    int before = *v;
    if (lal % definicion < y)
        *v -= diff;
    else *v += diff;

    // Next random
    static Uint8 red = 0xff;
    static Uint8 green = 0xff;
    static Uint8 blue = 0xff;

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

    const int sup = 255;
    const int inf = 55;
    const double k = 0.5;
    const int definition = 10000;
    //const int diff = 256 / 30;
    const int diff = 1;

    int y = (int) ((double)definition * f(*v, k, inf, sup));
    int lal = rand();
    int before = *v;
    if (lal % definition < y)
        *v -= diff;
    else *v += diff;

    debug("Chose color: y=%d, lal=%d, before=%d, after=%d", y, lal, before, *v);

    return (SDL_Color){.r=red, .g=green, .b=blue, .a=255};
}
*/

void setDrawColor(SDL_Renderer* ren, SDL_Color c) {
    SDL_SetRenderDrawColor(ren, c.r, c.g, c.b, c.a);
    return;
}

void* gameUpdater(void* arg) {
    debug("Initialized game updater thread");

    if (!arg) error("Game updater got null arg!");

    game_t* g = (game_t*) arg;

    while (g && g->running) {
        if (g->updating) update(g);
        SDL_Delay(g->delay);
    }

    debug("Finalized game updater thread");

    return NULL;
}

void handleEvents(game_t* game, graphic_context_t* gc, SDL_Event event) {
    static int rightClickSelection = -1;

    switch (event.type) {
    case SDL_EVENT_QUIT:
        game->running = false;
        info("Quitting...");
        break;

    case SDL_EVENT_MOUSE_MOTION:
        game->mouseX = event.motion.x;
        game->mouseY = event.motion.y;
        break;

    case SDL_EVENT_MOUSE_BUTTON_DOWN:
        SDL_Point cell = getCellOnThisPixel(gc, event.button.x, event.button.y);
        int c = cell.x;
        int r = cell.y;
        if (event.button.button == SDL_BUTTON_LEFT) rightClickSelection = r * game->grid->cols + c;
        break;

    case SDL_EVENT_MOUSE_BUTTON_UP:
        cell = getCellOnThisPixel(gc, event.button.x, event.button.y);
        c = cell.x;
        r = cell.y;

        if (event.button.button == SDL_BUTTON_LEFT) {
            if (rightClickSelection == (r * game->grid->cols + c)) {
                pthread_mutex_lock(&game->gridLock);
                ((bool*)game->grid->cells)[r * game->grid->cols + c] = !((bool*)game->grid->cells)[r * game->grid->cols + c];
                pthread_mutex_unlock(&game->gridLock);
                rightClickSelection = -1;
            }
        }

        break;

    case SDL_EVENT_KEY_DOWN:

        example_t* e = NULL;
        bool shouldLoadExample = false;

        switch (event.key.key) {
            case SDLK_SPACE:
                game->updating = !game->updating;
                if (!game->updating) info("Game stopped");
                else info("Game resumed");
                break;
            case SDLK_MINUS:
            case SDLK_KP_MINUS:
                int step = DELAY_TINY_STEP;
                if (event.key.mod & SDL_KMOD_CTRL)
                    step = DELAY_BIG_STEP;

                if (game->delay + step >= DELAY_TOP_LIMIT) warn("Not incrementing delay: must be lower than %d", DELAY_TOP_LIMIT);
                else info("Incremented delay: %d ms ", game->delay += step);
                break;
            case SDLK_PLUS:
            case SDLK_KP_PLUS:
                step = DELAY_TINY_STEP;
                if (event.key.mod & SDL_KMOD_CTRL)
                    step = DELAY_BIG_STEP;

                if (game->delay - step <= DELAY_BOTTOM_LIMIT) warn("Not decrementing delay: must be higher than %d", DELAY_BOTTOM_LIMIT);
                else info("Decremented delay: %d ms ", game->delay -= step);
                break;
            case SDLK_N:
                update(game);
                break;
            case SDLK_0:
                e = nothing();
                shouldLoadExample = true;
                break;
            case SDLK_1:
                e = chaos();
                shouldLoadExample = true;
                break;
            case SDLK_2:
                e = diehard();
                shouldLoadExample = true;
                break;
            case SDLK_3:
                e = glider();
                shouldLoadExample = true;
                break;
            case SDLK_4:
                e = acorn();
                shouldLoadExample = true;
                break;
            case SDLK_5:
                e = galaxy();
                shouldLoadExample = true;
                break;

            case SDLK_UP:
            case SDLK_LEFT:
            case SDLK_DOWN:
            case SDLK_RIGHT:
                direction_t direction;
                switch (event.key.key) {
                    case SDLK_UP: direction = DIRECTION_UP; break;
                    case SDLK_LEFT: direction = DIRECTION_LEFT; break;
                    case SDLK_DOWN: direction = DIRECTION_DOWN; break;
                    case SDLK_RIGHT: direction = DIRECTION_RIGHT; break;
                }
                processShift(game, gc, direction);
                break;

        } // END switch(event.key.keysym.sym) Qué botón se presionó?
        if (shouldLoadExample) {
            pthread_mutex_lock(&game->gridLock);
            loadExampleToGrid(game->grid, e);
            pthread_mutex_unlock(&game->gridLock);
            destroyExample(&e);
            break;
        }
    } // END switch(event.type) Qué evento fue?

}

void loadExample(game_t* game, example_t* example) {
    loadExampleToGrid(game->grid, example);
    return;
}

void destroyGame(game_t** game){
    game_t* g = *game;
    if (g != NULL) {
        freeGrid(&(g->grid));
        free(*game);
    }
    *game = NULL;
}

