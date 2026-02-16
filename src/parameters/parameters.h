#ifndef PARAMETERS_H
#define PARAMETERS_H

typedef struct {
    int rows;
    int cols;
    int blockSize;
    int fps;
    int delay;
} parameters_t;

parameters_t getDefaultParameters();

#endif
