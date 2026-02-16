#ifndef PARAMETERS_H
#define PARAMETERS_H

#include <stdlib.h>

// Null parameters are interpreted as if they were not set
typedef struct {
    int* rows;
    int* cols;
    int* blockSize;
    int* fps;
    int* delay;
} parameters_t;

parameters_t getNullParameters();
parameters_t getDefaultParameters();
parameters_t solveParameters(parameters_t defaultParams, parameters_t configParams, parameters_t optionsParams);

#endif
