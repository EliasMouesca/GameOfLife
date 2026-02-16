#ifndef OPTIONS_H
#define OPTIONS_H

#include <stdlib.h>
#include <unistd.h>

void usage(const char* prog);
void handleOptions(game_t* game, int argc, char* argv[]);

#endif
