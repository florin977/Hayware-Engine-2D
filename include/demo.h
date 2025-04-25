#ifndef DEMO_H

#define DEMO_H

#include "utils.h"

typedef struct POINT
{
    GLfloat x;
    GLfloat y;
} POINT;

#define MIN_SIDE 0.02f

void drawSquare(VECTOR *vector, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex);

void boxFractal(VECTOR *vertices, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex);

void printPoint(POINT *point);

#endif