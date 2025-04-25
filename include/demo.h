#ifndef DEMO_H

#define DEMO_H

#include "utils.h"

typedef struct POINT
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
} POINT;

#define SQUARE_POINTS 4
#define MIN_SIDE 0.025f

void drawSquare(VECTOR *vertices, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex);

void boxFractal(VECTOR *vertices, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex);

void printPoint(POINT *point);

#endif