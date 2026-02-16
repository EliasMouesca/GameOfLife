#ifndef CONFIG_H
#define CONFIG_H

#define MAX_LINE 256
#define MAX_KEYS 100
#define CONFIG_PATH_MAX_SIZE 256

#include "../utils/utils.h"
#include "../parameters/parameters.h"
#include "../options/options.h"

typedef struct {
    char key[50];
    char value[100];
} config_entry_t;

typedef struct {
    config_entry_t entries[MAX_KEYS];
    int count;
} config_t;

config_t* createConfig();
int parseConfig(char *filename, config_t *cfg);
parameters_t configToParameters(config_t* config);
void chooseAndOpenConfig(int argc, char* argv[], config_t *config);
char* getValue(config_t *cfg, const char *key);
void destroyConfig(config_t** config);


#endif
