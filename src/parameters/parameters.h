#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdlib.h>
#include <string.h>

typedef struct {
    int rows; bool rowsDefined;
    int cols; bool colsDefined;
    int blockSize; bool blockSizeDefined;
    int fps; bool fpsDefined;
    int delay; bool delayDefined;
    bool fullscreen; bool fullscreenDefined;
    bool party; bool partyDefined;
} parameters_t;

parameters_t getNullParameters();
parameters_t getSensibleDefaultParameters(int screenWidth, int screenHeight);
parameters_t getBaseDefaultParameters();
parameters_t solveParameters(parameters_t defaultParams, parameters_t configParams, parameters_t optionsParams, int windowWidth, int windowHeight);
bool areAllParametersSet(parameters_t params, char* missing);

#endif
