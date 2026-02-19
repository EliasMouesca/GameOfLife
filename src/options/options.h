#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <getopt.h>
#include <string.h>

#include "../parameters/parameters.h"
#include "../log/log.h"

#define GETOPT_STRING "r:c:b:fd:h"

#define LONG_OPTIONS struct option long_opts[] = {\
        {"rows",        required_argument,  0, 'r'},\
        {"cols",        required_argument,  0, 'c'},\
        {"block-size",  required_argument,  0, 'b'},\
        {"fps",         required_argument,  0, 0},\
        {"fullscreen",  no_argument,        0, 'f'},\
        {"party",       no_argument,        0, 'p'},\
        {"delay",       required_argument,  0, 'd'},\
        {"help",        no_argument,        0, 'h'},\
        {0, 0, 0, 0}\
    };


void usage(const char* prog);
parameters_t optionsToParameters(int argc, char* argv[]);

#endif
