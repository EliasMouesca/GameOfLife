#include "parameters.h"

parameters_t getDefaultParameters() {

    parameters_t p = {.rows=70, .cols=100, .blockSize=10, .fps=30, .delay=100};

    return p;
}

