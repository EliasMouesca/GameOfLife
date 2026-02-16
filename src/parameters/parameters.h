#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdlib.h>

typedef struct {
    int rows; bool rowsDefined;
    int cols; bool colsDefined;
    int blockSize; bool blockSizeDefined;
    int fps; bool fpsDefined;
    int delay; bool delayDefined;
} parameters_t;

parameters_t getNullParameters();
parameters_t getDefaultParameters();
parameters_t solveParameters(parameters_t defaultParams, parameters_t configParams, parameters_t optionsParams);

#endif
