
#include "game.h"
#include "pthread.h"

void* gameUpdater(void* arg);

game_t* createGame() {
    game_t* game = malloc(sizeof(game_t));
    return game;
}

void setGameConfig(game_t* game, config_t config) {
    const char* rowsStr = getValue(&config, "rows");
    const char* colsStr = getValue(&config, "cols");
    const char* blockSizeStr = getValue(&config, "block_size");
    const char* fpsStr = getValue(&config, "fps");
    const char* delayStr = getValue(&config, "delay");

    if (!rowsStr) die("Could not parse 'rows' config parameter");
    if (!colsStr) die("Could not parse 'cols' config parameter");
    if (!blockSizeStr) die("Could not parse 'blockSize' config parameter");
    if (!fpsStr) die("Could not parse 'fps' config parameter");
    if (!delayStr) die("Could not parse 'delay' config parameter");

    int rows = game->grid.rows = atoi(rowsStr);
    int cols = game->grid.cols = atoi(colsStr);
    game->blockSize = atoi(blockSizeStr);
    game->fps = atoi(fpsStr);
    game->delay = atoi(delayStr);

    void* cells = game->grid.cells = malloc(rows * cols * sizeof(bool));

    memset(cells, 0, rows * cols * sizeof(bool));

    example_t e = chaos();
    loadExample(game, e);
    destroyExample(&e);


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

void draw(game_t* game) {
    int rows = game->grid.rows;
    int cols = game->grid.cols;
    int blockSize = game->blockSize;
    SDL_SetRenderDrawColor(game->renderer, 0, 0, 0, 0xff);
    SDL_RenderClear(game->renderer);

    pthread_mutex_lock(&game->gridLock);
    grid_t grid = makeGrid(rows, cols, (bool*) game->grid.cells);
    pthread_mutex_unlock(&game->gridLock);

    SDL_SetRenderDrawColor(game->renderer, 0xff, 0xff, 0xff, 0xff);
    for (int y = 0; y < rows; y++)
    for (int x = 0; x < cols; x++)
        if (grid.cells[y * cols + x]) {
            SDL_Rect rect = {x * blockSize, y * blockSize, blockSize, blockSize};
            SDL_RenderFillRect(game->renderer, &rect);
        }

    if ( (game->mouseX > 0 && game->mouseX < grid.cols * blockSize) && 
            (game->mouseY > 0 && game->mouseY < grid.rows * blockSize) )
    {
        int x = game->mouseX - (game->mouseX % blockSize);
        int y = game->mouseY - (game->mouseY % blockSize);

        SDL_Rect rect = {x, y, blockSize, blockSize};
        SDL_SetRenderDrawColor(game->renderer, 0xaa, 0xaa, 0xaa, 0xff);
        SDL_RenderFillRect(game->renderer, &rect);
    }

    freeGrid(grid);

    SDL_RenderPresent(game->renderer);

    return;

}

void* gameUpdater(void* arg) {
    game_t* game = (game_t*) arg;

    while (game->updating && game->running) {
        update(game);
        SDL_Delay(game->delay);
    }

    return NULL;
}

void handleEvents(game_t* game, SDL_Event event) {
    pthread_t thread;

    switch (event.type) {
    case SDL_QUIT:
        game->running = false;
        break;

    case SDL_MOUSEMOTION:
        game->mouseX = event.motion.x;
        game->mouseY = event.motion.y;
        break;

    case SDL_MOUSEBUTTONDOWN:
        int c = event.button.x / game->blockSize;
        int r = event.button.y / game->blockSize;

        pthread_mutex_lock(&game->gridLock);
        game->grid.cells[r * game->grid.cols + c] = !game->grid.cells[r * game->grid.cols + c];
        pthread_mutex_unlock(&game->gridLock);

        break;

    case SDL_KEYDOWN:

        example_t e;
        bool shouldLoadExample = false;

        switch (event.key.keysym.sym) {
            case SDLK_SPACE:
                game->updating = !game->updating;
                if (game->updating)
                    pthread_create(&thread, NULL, gameUpdater, game);
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

    return;

}

void destroyGame(game_t* game){
    freeGrid(game->grid);
    free(game);
}

