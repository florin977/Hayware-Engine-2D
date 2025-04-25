#ifndef UTILS_H

#define UTILS_H

//#include "render.h"
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/gl.h>

#define CHUNK 1

typedef struct VECTOR
{
    GLuint64 size;
    GLuint64 currentIndex;
    uint8_t dataSize;
    void *array;
} VECTOR;

VECTOR createVector(uint8_t dataSize); // Call createVector after malloc == memory leak

void push_back(VECTOR *v, void *element);

void *getElement(VECTOR *v, GLuint64 index);

void push_back_old(void **array, GLuint64 *size, GLuint64 *position, void *element, uint8_t dataSize);

#endif