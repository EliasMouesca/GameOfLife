#include "log.h"

#include <stdio.h>
#include <stdarg.h>
#include <time.h>

// MISMO enum (no tocar)
typedef enum {
    TRACE,
    DEBUG,
    INFO,
    WARN,
    ERROR,
    CRITICAL
} log_level_t;

static const char* level_names[] = {
    "TRACE",
    "DEBUG",
    "INFO",
    "WARN",
    "ERROR",
    "CRITICAL"
};

static const char* level_colors[] = {
    "\033[90m",
    "\033[94m",
    "\033[36m",
    "\033[33m",
    "\033[31m",
    "\033[41m\033[97m"
};

static const char* reset = "\033[0m";


// ---------------------------------------------------------
//  log_msg: versión variádica "normal"
// ---------------------------------------------------------
void log_msg(log_level_t level, const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);

    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    fprintf(stderr, "%s[%02d:%02d:%02d] %-8s ",
            level_colors[level],
            tm->tm_hour, tm->tm_min, tm->tm_sec,
            level_names[level]);

    vfprintf(stderr, fmt, args);

    fprintf(stderr, "%s\n", reset);

    va_end(args);
}


// ---------------------------------------------------------
//  WRAPPERS: NO pueden llamar log_msg() correctamente.
//  Así que repiten la misma lógica.
// ---------------------------------------------------------
static void log_wrapper(log_level_t level, const char* fmt, va_list args) {
    time_t t = time(NULL);
    struct tm* tm = localtime(&t);

    fprintf(stderr, "%s[%02d:%02d:%02d] %-8s ",
            level_colors[level],
            tm->tm_hour, tm->tm_min, tm->tm_sec,
            level_names[level]);

    vfprintf(stderr, fmt, args);

    fprintf(stderr, "%s\n", reset);
}

void trace(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_wrapper(TRACE, fmt, args);
    va_end(args);
}

void debug(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_wrapper(DEBUG, fmt, args);
    va_end(args);
}

void info(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_wrapper(INFO, fmt, args);
    va_end(args);
}

void warn(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_wrapper(WARN, fmt, args);
    va_end(args);
}

void error(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_wrapper(ERROR, fmt, args);
    va_end(args);
}

void critical(const char* fmt, ...) {
    va_list args;
    va_start(args, fmt);
    log_wrapper(CRITICAL, fmt, args);
    va_end(args);
}

