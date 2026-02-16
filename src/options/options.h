#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdlib.h>
#include <unistd.h>

#include "../parameters/parameters.h"

void usage(const char* prog);
parameters_t optionsToParameters();

#endif
