#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdarg.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

void die(const char* fmt, ...);
void trim(char* str);

#endif
