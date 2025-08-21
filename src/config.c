#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "config.h"
#include "utils.h"

#define MAX_LINE 256
#define MAX_KEYS 100

int parseConfig(const char *filename, config_t *cfg) {
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

const char* getValue(config_t *cfg, const char *key) {
    for (int i = 0; i < cfg->count; i++) {
        if (strcmp(cfg->entries[i].key, key) == 0)
            return cfg->entries[i].value;
    }
    return NULL;
}

