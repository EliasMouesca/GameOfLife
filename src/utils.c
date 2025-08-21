//
// Created by eli on 19/8/25.
//

#include "utils.h"

#include <stdlib.h>

void die(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");

    exit(EXIT_FAILURE);
}

void trim(char* str) {
    while (isspace((unsigned char)*str)) str++;

    char *end = str + strlen(str) - 1;
    while (end > str && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';
}
