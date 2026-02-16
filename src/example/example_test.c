#include "example.h"
#include <assert.h>

int main() {

    example_t example = nothing();

    assert(example.rows == 0);
    assert(example.rows == 0);
    assert(example.hpos == LEFT);
    assert(example.vpos == TOP);
    assert(example.cells == NULL);

    destroyExample(&example);

    return 0;
}
