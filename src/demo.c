#include "demo.h"

void drawSquare(VECTOR *vector, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex)
{
    GLuint startIndex = *currentIndex;

    POINT topLeft = {start.x, start.y};
    POINT topRight = {start.x + side, start.y};
    POINT bottomRight = {start.x + side, start.y - side};
    POINT bottomLeft = {start.x, start.y - side};
    
    push_back(vector, &topLeft);
    push_back(indices, currentIndex);
    (*currentIndex)++;

    push_back(vector, &topRight);
    push_back(indices, currentIndex);
    push_back(indices, currentIndex);
    (*currentIndex)++;

    push_back(vector, &bottomRight);
    push_back(indices, currentIndex);
    push_back(indices, currentIndex);
    (*currentIndex)++;

    push_back(vector, &bottomLeft);
    push_back(indices, currentIndex);
    push_back(indices, currentIndex);
    push_back(indices, &startIndex);
    (*currentIndex)++;
}

void boxFractal(VECTOR *vertices, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex)
{
    if (side <= MIN_SIDE)
    {
        drawSquare(vertices, indices, start, side, currentIndex);
    }
    else 
    {
        side = side / 3;

        POINT topLeft = {start.x, start.y};
        POINT topRight = {start.x + 2 * side, start.y};
        POINT center = {start.x + side, start.y - side};
        POINT bottomLeft = {start.x, start.y - 2 * side};
        POINT bottomRight = {start.x + 2 * side, start.y - 2 * side};

        boxFractal(vertices, indices, topLeft, side, currentIndex);

        boxFractal(vertices, indices, topRight, side, currentIndex);
        
        boxFractal(vertices, indices, center, side, currentIndex);

        boxFractal(vertices, indices, bottomLeft, side, currentIndex);

        boxFractal(vertices, indices, bottomRight, side, currentIndex);
    }
}

void printPoint(POINT *point)
{
    printf("%f ", point->x);
    printf("%f\n", point->y);
}