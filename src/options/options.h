#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>

#include "../parameters/parameters.h"
#include "../log/log.h"

void usage(const char* prog);
parameters_t optionsToParameters(int argc, char* argv[]);

#endif
