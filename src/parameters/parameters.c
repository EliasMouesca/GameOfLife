#include "parameters.h"

#define DEFAULT_ROWS 70
#define DEFAULT_COLS 100
#define DEFAULT_BLOCK_SIZE 10
#define DEFAULT_FPS 30
#define DEFAULT_DELAY 100

static int chooseInt(bool *defined, int a, bool aDef, int b, bool bDef, int c, bool cDef);

parameters_t getBaseDefaultParameters() {
    return (parameters_t){
        .rows = DEFAULT_ROWS,
        .rowsDefined = true,

        .cols = DEFAULT_COLS,
        .colsDefined = true,

        .blockSize = DEFAULT_BLOCK_SIZE,
        .blockSizeDefined = true,

        .fps = DEFAULT_FPS,
        .fpsDefined = true,

        .delay = DEFAULT_DELAY,
        .delayDefined = true,
    };
}

parameters_t getSensibleDefaultParameters(int screenWidth, int screenHeight) {
    return (parameters_t){
        .rows = (screenHeight / DEFAULT_BLOCK_SIZE) * 9 / 10,
        .rowsDefined = true,

        .cols = (screenWidth / DEFAULT_BLOCK_SIZE) * 9 / 10,
        .colsDefined = true,

        .blockSize = DEFAULT_BLOCK_SIZE,
        .blockSizeDefined = true,

        .fps = DEFAULT_FPS,
        .fpsDefined = true,

        .delay = DEFAULT_DELAY,
        .delayDefined = true,
    };
}

parameters_t getNullParameters() {
    return (parameters_t){0};
}

parameters_t solveParameters(parameters_t defaultParams, parameters_t configParams, parameters_t optionsParams) {
    parameters_t p = getNullParameters();

    p.rows = chooseInt(&p.rowsDefined,
                   defaultParams.rows,  defaultParams.rowsDefined,
                   configParams.rows,   configParams.rowsDefined,
                   optionsParams.rows,  optionsParams.rowsDefined);

    p.cols = chooseInt(&p.colsDefined,
                   defaultParams.cols,  defaultParams.colsDefined,
                   configParams.cols,   configParams.colsDefined,
                   optionsParams.cols,  optionsParams.colsDefined);

    p.blockSize = chooseInt(&p.blockSizeDefined,
                        defaultParams.blockSize, defaultParams.blockSizeDefined,
                        configParams.blockSize,  configParams.blockSizeDefined,
                        optionsParams.blockSize, optionsParams.blockSizeDefined);

    p.fps = chooseInt(&p.fpsDefined,
                  defaultParams.fps,  defaultParams.fpsDefined,
                  configParams.fps,   configParams.fpsDefined,
                  optionsParams.fps,  optionsParams.fpsDefined);

    p.delay = chooseInt(&p.delayDefined,
                    defaultParams.delay,  defaultParams.delayDefined,
                    configParams.delay,   configParams.delayDefined,
                    optionsParams.delay,  optionsParams.delayDefined);


    return p;
}


static int chooseInt(bool *defined,
                     int a, bool aDef,
                     int b, bool bDef,
                     int c, bool cDef)
{
    if (cDef) { *defined = true; return c; }
    if (bDef) { *defined = true; return b; }
    if (aDef) { *defined = true; return a; }
    *defined = false;
    return 0;
}


