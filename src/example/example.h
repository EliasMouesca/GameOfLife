#ifndef EXAMPLE_H
#define EXAMPLE_H

#include <stdbool.h>

#include "../grid/grid.h"

typedef struct example_t example_t;

example_t* nothing();
example_t* chaos();
example_t* diehard();
example_t* glider();
example_t* acorn();
example_t* galaxy();
void loadExampleToGrid(grid_t* grid, example_t* example);
void destroyExample(example_t** example);

#endif
