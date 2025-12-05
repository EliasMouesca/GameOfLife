
#include "game.h"

#include "log.h"

#define DELAY_TINY_STEP 10
#define DELAY_BIG_STEP 25
#define DELAY_TOP_LIMIT 1000
#define DELAY_BOTTOM_LIMIT 0

void* gameUpdater(void* arg);

game_t* createGame() {
    game_t* game = malloc(sizeof(game_t));
    game->updating = false;
    game->running = true;
    game->delay = 0;
    pthread_mutex_init(&game->gridLock, NULL);

    return game;
}

void setGameConfig(game_t* game, config_t config) {
    const char* rowsStr = getValue(&config, "rows");
    const char* colsStr = getValue(&config, "cols");
    const char* delayStr = getValue(&config, "delay");

    if (!rowsStr || rowsStr[0]=='\0') die("Could not parse 'rows' config parameter");
    if (!colsStr || colsStr[0]=='\0') die("Could not parse 'cols' config parameter");
    if (!delayStr || delayStr[0]=='\0') die("Could not parse 'delay' config parameter");

    int rows = game->grid.rows = atoi(rowsStr);
    int cols = game->grid.cols = atoi(colsStr);
    game->delay = atoi(delayStr);

    game->grid.cells = malloc(rows * cols * sizeof(bool));

    memset(game->grid.cells, 0, rows * cols * sizeof(bool));

}

void beginUpdating(game_t* game) {
    pthread_create(&game->updaterThread, NULL, gameUpdater, game);
}

void figureSensibleDefaults(game_t* game, graphic_context_t* gc) {
    int blockSize = 15;
    int fps = 60;
    int delay = 100;

    gc->blockSize = blockSize;
    gc->fps = fps;
    game->delay = delay;

    SDL_DisplayMode DM;
    SDL_GetCurrentDisplayMode(0, &DM);

    int cols = (0.8 * DM.w) / blockSize;
    int rows = (0.8 * DM.h) / blockSize;
    int windowWidth = cols * blockSize;
    int windowHeight = rows * blockSize;
    int windowFlags = 0;

    game->grid.rows = rows;
    game->grid.cols = cols;
    void* cells = game->grid.cells = malloc(rows * cols * sizeof(bool));
    memset(cells, 0, rows * cols * sizeof(bool));

    setGraphicContextParams(gc, windowWidth, windowHeight, windowFlags, blockSize, fps);

}

void update(game_t* game) {
    pthread_mutex_lock(&game->gridLock);
    grid_t grid;
    memcpy(&grid, &(game->grid), sizeof(grid_t));
    bool* buffer = (bool*) malloc(grid.rows * grid.cols * sizeof(bool));

    for (int y = 0; y < grid.rows; y++)
    for (int x = 0; x < grid.cols; x++) {
        int i = y * grid.cols + x;
        int neighborsAlive = 0;

        if ((y > 0 && x > 0) ? grid.cells[i-grid.cols-1] : false) neighborsAlive++;
        if ((y > 0) ? grid.cells[i-grid.cols] : false) neighborsAlive++;
        if ((y > 0 && x < grid.cols - 1) ? grid.cells[i-grid.cols + 1] : false) neighborsAlive++;
        
        if ((x > 0) ? grid.cells[i-1] : false) neighborsAlive++;
        if ((x < grid.cols - 1) ? grid.cells[i+1] : false) neighborsAlive++;

        if ((y < grid.rows - 1 && x > 0) ? grid.cells[i+grid.cols-1] : false) neighborsAlive++;
        if ((y < grid.rows - 1) ? grid.cells[i+grid.cols] : false) neighborsAlive++;
        if ((y < grid.rows - 1 && x < grid.cols - 1) ? grid.cells[i+grid.cols+1] : false) neighborsAlive++;

        if (grid.cells[i]) {
            if (neighborsAlive == 2 || neighborsAlive == 3) buffer[i] = true;
            else buffer[i] = false;
        } else {
            if (neighborsAlive == 3) buffer[i] = true;
            else buffer[i] = false;
        }
    }

    memcpy(grid.cells, buffer, grid.rows * grid.cols * sizeof(bool));
    pthread_mutex_unlock(&game->gridLock);

    free(buffer);

    return;
}

void draw(game_t* game, graphic_context_t* gc) {
    int rows = game->grid.rows;
    int cols = game->grid.cols;
    int blockSize = gc->blockSize;
    SDL_SetRenderDrawColor(gc->renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(gc->renderer);

    pthread_mutex_lock(&game->gridLock);
    grid_t grid = makeGrid(rows, cols, (bool*) game->grid.cells);
    pthread_mutex_unlock(&game->gridLock);

    SDL_SetRenderDrawColor(gc->renderer, 0xff, 0xff, 0xff, 0xff);
    for (int y = 0; y < rows; y++)
    for (int x = 0; x < cols; x++)
        if (grid.cells[y * cols + x]) {
            SDL_Rect rect = {x * blockSize, y * blockSize, blockSize, blockSize};
            SDL_RenderFillRect(gc->renderer, &rect);
        }

    if ( (game->mouseX > 0 && game->mouseX < grid.cols * blockSize) && 
            (game->mouseY > 0 && game->mouseY < grid.rows * blockSize) )
    {
        int x = game->mouseX - (game->mouseX % blockSize);
        int y = game->mouseY - (game->mouseY % blockSize);

        SDL_Rect rect = {x, y, blockSize, blockSize};
        SDL_SetRenderDrawColor(gc->renderer, 0xaa, 0xaa, 0xaa, 0xff);
        SDL_RenderFillRect(gc->renderer, &rect);
    }

    freeGrid(grid);

    SDL_RenderPresent(gc->renderer);

    return;

}

void* gameUpdater(void* arg) {
    debug("Initialized game updater thread");
    game_t* g = (game_t*) arg;

    while (g->running) {
        if (g->updating) update(g);
        SDL_Delay(g->delay);
    }

    debug("Finalized game updater thread");

    return NULL;
}

void handleEvents(game_t* game, graphic_context_t* gc, SDL_Event event) {
    static int rightClickSelection = -1;

    switch (event.type) {
    case SDL_QUIT:
        game->running = false;
        info("Quitting...");
        break;

    case SDL_MOUSEMOTION:
        game->mouseX = event.motion.x;
        game->mouseY = event.motion.y;
        break;

    case SDL_MOUSEBUTTONDOWN:
        int c = event.button.x / gc->blockSize;
        int r = event.button.y / gc->blockSize;
        if (event.button.button == SDL_BUTTON_LEFT) rightClickSelection = r * game->grid.cols + c;
        break;

    case SDL_MOUSEBUTTONUP:
        c = event.button.x / gc->blockSize;
        r = event.button.y / gc->blockSize;

        if (event.button.button == SDL_BUTTON_LEFT) {
            if (rightClickSelection == (r * game->grid.cols + c)) {
                pthread_mutex_lock(&game->gridLock);
                game->grid.cells[r * game->grid.cols + c] = !game->grid.cells[r * game->grid.cols + c];
                pthread_mutex_unlock(&game->gridLock);
                rightClickSelection = -1;
            }
        }

        break;

    case SDL_KEYDOWN:

        example_t e;
        bool shouldLoadExample = false;

        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                game->updating = !game->updating;
                if (!game->updating) info("Game stopped");
                else info("Game resumed");
                break;
            case SDLK_MINUS:
            case SDLK_KP_MINUS:
                int step = DELAY_TINY_STEP;
                if (event.key.keysym.mod & KMOD_CTRL)
                    step = DELAY_BIG_STEP;

                if (game->delay + step >= DELAY_TOP_LIMIT) warn("Not incrementing delay: must be lower than %d", DELAY_TOP_LIMIT);
                else info("Incremented delay: %d ms ", game->delay += step);
                break;
            case SDLK_PLUS:
            case SDLK_KP_PLUS:
                step = DELAY_TINY_STEP;
                if (event.key.keysym.mod & KMOD_CTRL)
                    step = DELAY_BIG_STEP;

                if (game->delay - step <= DELAY_BOTTOM_LIMIT) warn("Not decrementing delay: must be higher than %d", DELAY_BOTTOM_LIMIT);
                else info("Decremented delay: %d ms ", game->delay -= step);
                break;
            case SDLK_n:
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
                pthread_mutex_lock(&game->gridLock);
                direction_t direction;
                switch (event.key.keysym.sym) {
                    case SDLK_UP: direction = DIRECTION_UP; break;
                    case SDLK_LEFT: direction = DIRECTION_LEFT; break;
                    case SDLK_DOWN: direction = DIRECTION_DOWN; break;
                    case SDLK_RIGHT: direction = DIRECTION_RIGHT; break;
                }
                shiftGrid(game->grid, direction);
                pthread_mutex_unlock(&game->gridLock);
                break;

        } // END switch(event.key.keysym.sym) Qué botón se presionó?
        if (shouldLoadExample) {
            loadExample(game, e);
            destroyExample(&e);
            break;
        }
    } // END switch(event.type) Qué evento fue?

}


void loadExample(game_t* game, example_t example) {

    int cols = game->grid.cols;
    int rows = game->grid.rows;

    pthread_mutex_lock(&game->gridLock);
    memset(game->grid.cells, false, cols * rows * sizeof(bool));

    int startingX = 0;
    int startingY = 0;

    switch (example.hpos) {
        case LEFT: startingX = 0; break;
        case CENTER: startingX = cols / 2 - example.cols / 2; break;
        case RIGHT: startingX = cols - example.cols; break;
    }

    switch (example.vpos) {
        case TOP: startingY = 0; break;
        case MIDDLE: startingY = rows / 2 - example.rows / 2; break;
        case BOTTOM: startingY = rows - example.rows; break;
    }

    for (int y = 0; y < example.rows; y++)
    for (int x = 0; x < example.cols; x++) {
        if ( example.cells[y * example.cols + x] )
            game->grid.cells[(startingY + y) * cols + startingX + x] = true;
    }

    pthread_mutex_unlock(&game->gridLock);

    info("Loaded example to grid");

    return;

}

void destroyGame(game_t* game){
    freeGrid(game->grid);
    free(game);
}

