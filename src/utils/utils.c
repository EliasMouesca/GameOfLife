#include "utils.h"

void die(const char* fmt, ...) {
    va_list ap;
    va_start(ap, fmt);
    vprintf(fmt, ap);
    va_end(ap);
    printf("\n");

    exit(EXIT_FAILURE);
}

void trim(char* s) {
    char *start = s;
    while (isspace((unsigned char)*start)) start++;

    if (*start == 0) {
        *s = 0;
        return;
    }

    char *end = start + strlen(start) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;
    *(end + 1) = '\0';

    memmove(s, start, end - start + 2);
}

