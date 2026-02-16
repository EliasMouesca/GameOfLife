#include "parameters.h"

#define DEFAULT_ROWS 70
#define DEFAULT_COLS 100
#define DEFAULT_BLOCK_SIZE 10
#define DEFAULT_FPS 30
#define DEFAULT_DELAY 100

parameters_t getDefaultParameters() {
    int* rows = malloc(sizeof(int));
    *rows = DEFAULT_ROWS;
    int* cols = malloc(sizeof(int));
    *cols = DEFAULT_COLS;
    int* blockSize = malloc(sizeof(int));
    *blockSize = DEFAULT_BLOCK_SIZE;
    int* fps = malloc(sizeof(int));
    *fps = DEFAULT_FPS;
    int* delay = malloc(sizeof(int));
    *delay = DEFAULT_DELAY;

    parameters_t p = {.rows=rows, .cols=cols, .blockSize=blockSize, .fps=fps, .delay=delay};

    return p;
}

parameters_t solveParameters(parameters_t defaultParams, parameters_t configParams, parameters_t optionsParams) {
    return getDefaultParameters();
}

parameters_t getNullParameters() {
    parameters_t p ={.rows=NULL, .cols=NULL, .blockSize=NULL, .fps=NULL, .delay=NULL};
    return p;
}

