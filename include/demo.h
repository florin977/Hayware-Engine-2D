#ifndef DEMO_H

#define DEMO_H

#include "utils.h"

#define SQUARE_POINTS 4
#define CONNECTIONS_NUM 4
#define MIN_SIDE 0.05f

void drawSquare(VECTOR *vertices, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex);

void drawSquareFullFace(VECTOR *vertices, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex);

void boxFractal(VECTOR *vertices, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex);

void printPoint(POINT *point);

#endif