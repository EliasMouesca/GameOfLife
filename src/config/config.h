#ifndef CONFIG_H
#define CONFIG_H

#define MAX_LINE 256
#define MAX_KEYS 100

#include "../utils/utils.h"
#include "../parameters/parameters.h"

typedef struct {
    char key[50];
    char value[100];
} config_entry_t;

typedef struct {
    config_entry_t entries[MAX_KEYS];
    int count;
} config_t;

int parseConfig(const char *filename, config_t *cfg);
parameters_t configToParameters(config_t* config);
const char* getValue(config_t *cfg, const char *key);


#endif
