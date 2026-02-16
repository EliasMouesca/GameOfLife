#include "config.h"

#include <stdlib.h>
#include <assert.h>
#include <string.h>

int main() {
    config_t* config = malloc(sizeof(config_t));

    int parseResult = parseConfig("src/config/test_config.txt", config);
    assert(parseResult == 0);

    const char* rows = getValue(config, "rows");
    assert(strcmp(rows, "70") == 0);

    const char* cols = getValue(config, "cols");
    assert(strcmp(cols, "100") == 0);

    const char* blockSize = getValue(config, "block_size");
    assert(strcmp(blockSize, "10") == 0);

    const char* fps = getValue(config, "fps");
    assert(strcmp(fps, "30") == 0);

    const char* delay = getValue(config, "delay");
    assert(strcmp(delay, "100") == 0);

    puts("Passed!");

    return 0;
}


