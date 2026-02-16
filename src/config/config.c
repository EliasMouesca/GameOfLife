#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "config.h"

#define MAX_LINE 256
#define MAX_KEYS 100

char* chooseConfigFile(int argc, char* argv[]);
int parseConfig(char* filename, config_t *cfg);

config_t* createConfig(void) {
    config_t *cfg = (config_t*) malloc(sizeof(config_t));
    if (!cfg) return NULL;

    memset(cfg, 0, sizeof(config_t));

    return cfg;
}

void destroyConfig(config_t **config) {
    if (!config || !*config) return;

    free(*config);
    *config = NULL;
}


void chooseAndOpenConfig(int argc, char* argv[], config_t *config) {
    char* filename = chooseConfigFile(argc, argv);
    if (filename) {
        parseConfig(filename, config);
    }

    return;
}

char* chooseConfigFile(int argc, char* argv[]) {
    struct option long_opts[] = {
        {"rows",        required_argument, 0, 'r'},
        {"cols",        required_argument, 0, 'c'},
        {"block-size",  required_argument, 0, 'b'},
        {"fps",         required_argument, 0, 'f'},
        {"delay",       required_argument, 0, 'd'},
        {"help",        no_argument,       0, 'h'},
        {0, 0, 0, 0}
    };

    optind = 1;
    while (getopt_long(argc, argv, GETOPT_STRING, long_opts, NULL) != -1);

    char* configPath = malloc(sizeof(char) * CONFIG_PATH_MAX_SIZE);

    if (optind < argc) {
        size_t size = strlen(argv[optind]);
        if (size < CONFIG_PATH_MAX_SIZE) strncpy(configPath, argv[optind], CONFIG_PATH_MAX_SIZE - 1);
        else critical("Config path '%s' too large (max: %d characters)", argv[1], CONFIG_PATH_MAX_SIZE);
    } else return NULL;

    debug("Config provided, reading parameters from '%s' ...", configPath);

    return configPath;

}

int parseConfig(char* filename, config_t *cfg) {
    FILE *f = fopen(filename, "r");
    if (!f) return -1;

    char line[MAX_LINE];
    cfg->count = 0;

    while (fgets(line, sizeof(line), f)) {
        trim(line);
        if (line[0] == '#' || line[0] == ';' || line[0] == '\0') continue;

        char *eq = strchr(line, '=');
        if (!eq) continue; // skip invalid lines
        *eq = '\0';
        trim(line);
        trim(eq + 1);

        if (cfg->count >= MAX_KEYS) break;
        strncpy(cfg->entries[cfg->count].key, line, sizeof(cfg->entries[cfg->count].key));
        strncpy(cfg->entries[cfg->count].value, eq + 1, sizeof(cfg->entries[cfg->count].value));
        cfg->count++;
    }

    fclose(f);
    return 0;
}

parameters_t configToParameters(config_t* config) {
    parameters_t params = getNullParameters();
    if (config->count == 0) return params;

    char* buffer;
    buffer = getValue(config, "rows");
    if (buffer) {
        params.rowsDefined = true;
        params.rows = atoi(buffer);
    }

    buffer = getValue(config, "cols");
    if (buffer) {
        params.colsDefined = true;
        params.cols = atoi(buffer);
    }

    buffer = getValue(config, "block_size");
    if (buffer) {
        params.blockSize = true;
        params.blockSize = atoi(buffer);
    }

    buffer = getValue(config, "fps");
    if (buffer) {
        params.fpsDefined = true;
        params.fps = atoi(buffer);
    }

    buffer = getValue(config, "delay");
    if (buffer) {
        params.delayDefined = true;
        params.delay  = atoi(buffer);
    }

    return params;
}

char* getValue(config_t *cfg, const char *key) {
    for (int i = 0; i < cfg->count; i++) {
        if (strcmp(cfg->entries[i].key, key) == 0)
            return cfg->entries[i].value;
    }
    return NULL;
}

