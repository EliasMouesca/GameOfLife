#include "parameters.h"

#define DEFAULT_ROWS 70
#define DEFAULT_COLS 100
#define DEFAULT_BLOCK_SIZE 17
#define DEFAULT_FPS 30
#define DEFAULT_DELAY 100
#define DEFAULT_FULLSCREEN false
#define DEFAULT_PARTY false

static int chooseInt(bool *defined, int a, bool aDef, int b, bool bDef, int c, bool cDef);
static bool chooseBool(bool *defined, bool a, bool aDef, bool b, bool bDef, bool c, bool cDef);

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

        .fullscreen = DEFAULT_FULLSCREEN,
        .fullscreenDefined = true,

        .party = DEFAULT_PARTY,
        .partyDefined = true,

    };
}

parameters_t getSensibleDefaultParameters(int screenWidth, int screenHeight) {
    // Somewhat random, but I think it works
    int blockSize = screenHeight / 85;

    return (parameters_t){
        .rows = (screenHeight / blockSize) * 8 / 10,
        .rowsDefined = true,

        .cols = (screenWidth / blockSize) * 8 / 10,
        .colsDefined = true,

        .blockSize = blockSize,
        .blockSizeDefined = true,

        .fps = DEFAULT_FPS,
        .fpsDefined = true,

        .delay = DEFAULT_DELAY,
        .delayDefined = true,

        .fullscreen = DEFAULT_FULLSCREEN,
        .fullscreenDefined = true,

        .party = DEFAULT_PARTY,
        .partyDefined = true,

    };
}

parameters_t getNullParameters() {
    return (parameters_t){0};
}

bool areAllParametersSet(parameters_t params, char* buffer) {
    if (!params.colsDefined) {
        if (buffer) strcpy(buffer, "cols");
        return false;
    }

    if (!params.rowsDefined) {
        if (buffer) strcpy(buffer, "rows");
        return false;
    }

    if (!params.blockSizeDefined) {
        if (buffer) strcpy(buffer, "blockSize");
        return false;
    }

    if (!params.fpsDefined){
        if (buffer) strcpy(buffer, "fps");
        return false;
    }

    if (!params.delayDefined){
        if (buffer) strcpy(buffer, "delay");
        return false;
    }

    if (!params.fullscreenDefined){
        if (buffer) strcpy(buffer, "fullscreen");
        return false;
    }
    
    if (!params.partyDefined){
        if (buffer) strcpy(buffer, "party");
        return false;
    }

    return true;

}

parameters_t solveParameters(parameters_t defaultParams, parameters_t configParams, parameters_t optionsParams, int screenWidth, int screenHeight) {
    parameters_t p = getNullParameters();

    p.fullscreen = chooseBool(&p.fullscreenDefined,
                    defaultParams.fullscreen,  defaultParams.fullscreenDefined,
                    configParams.fullscreen,   configParams.fullscreenDefined,
                    optionsParams.fullscreen,  optionsParams.fullscreenDefined);

    p.blockSize = chooseInt(&p.blockSizeDefined,
                        defaultParams.blockSize, defaultParams.blockSizeDefined,
                        configParams.blockSize,  configParams.blockSizeDefined,
                        optionsParams.blockSize, optionsParams.blockSizeDefined);

    if (p.fullscreenDefined && p.fullscreen) {
        p.cols = screenWidth / p.blockSize;
        p.colsDefined = true;
        p.rows = screenHeight / p.blockSize;
        p.rowsDefined = true;
    } else {
        p.rows = chooseInt(&p.rowsDefined,
                       defaultParams.rows,  defaultParams.rowsDefined,
                       configParams.rows,   configParams.rowsDefined,
                       optionsParams.rows,  optionsParams.rowsDefined);

        p.cols = chooseInt(&p.colsDefined,
                       defaultParams.cols,  defaultParams.colsDefined,
                       configParams.cols,   configParams.colsDefined,
                       optionsParams.cols,  optionsParams.colsDefined);
    }

    p.fps = chooseInt(&p.fpsDefined,
                  defaultParams.fps,  defaultParams.fpsDefined,
                  configParams.fps,   configParams.fpsDefined,
                  optionsParams.fps,  optionsParams.fpsDefined);

    p.delay = chooseInt(&p.delayDefined,
                    defaultParams.delay,  defaultParams.delayDefined,
                    configParams.delay,   configParams.delayDefined,
                    optionsParams.delay,  optionsParams.delayDefined);

    p.party = chooseBool(&p.partyDefined,
                    defaultParams.party,  defaultParams.party,
                    configParams.party,   configParams.party,
                    optionsParams.party,  optionsParams.party);


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

static bool chooseBool(bool *defined,
                     bool a, bool aDef,
                     bool b, bool bDef,
                     bool c, bool cDef)
{
    if (cDef) { *defined = true; return c; }
    if (bDef) { *defined = true; return b; }
    if (aDef) { *defined = true; return a; }
    *defined = false;
    return 0;
}

