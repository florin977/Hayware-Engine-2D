#ifndef BREAKOUT_H

#define BREAKOUT_H

#include "utils.h"
#include "render.h"

typedef struct CANVAS
{
    VECTOR vertices;
    VECTOR indices;
} CANVAS;

extern CANVAS canvas;

CANVAS createCanvas();

#endif