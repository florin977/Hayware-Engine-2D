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

#define PI 3.14
#define CHUNK 1

typedef struct VECTOR
{
    GLuint64 size;
    GLuint64 currentIndex;
    uint8_t dataSize;
    void *array;
} VECTOR;


typedef struct POINT
{
    GLfloat x;
    GLfloat y;
    GLfloat z;
} POINT;

typedef struct COLOR
{
    GLfloat r;
    GLfloat g;
    GLfloat b;
} COLOR;

typedef struct TEXTURE
{
    GLfloat s; // x
    GLfloat t; // y
    GLfloat r; // z - Used for noise generation, volumetric fog etc.
} TEXTURE;

typedef struct VERTEX
{
    // Position
    POINT position;

    // Color
    COLOR color;

    // Texture coordinates
    TEXTURE texture;
} VERTEX;

VECTOR createVector(uint8_t dataSize); // Call createVector after malloc == memory leak

VERTEX createVertex(); // Position: (0, 0, 0), color: white, texture coords: (0, 0)

void createFullscreenQuad(VECTOR *vertices, VECTOR *indices);

void push_back(VECTOR *v, void *element);

void *getElement(VECTOR *v, GLuint64 index);

GLdouble toRadians(GLdouble degrees);

#endif