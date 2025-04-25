#ifndef DEMO_H

#define DEMO_H

#include "utils.h"

typedef struct POINT
{
    GLfloat x;
    GLfloat y;
} POINT;

void drawSquare(VECTOR *vector, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex);

void boxFractal(VECTOR *vertices, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex, GLuint depth);

void printPoint(POINT *point);

#endif