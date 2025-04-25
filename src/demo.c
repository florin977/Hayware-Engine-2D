#include "demo.h"

void drawSquare(VECTOR *vector, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex)
{
    GLuint startIndex = *currentIndex;
    GLuint backStartIndex = *currentIndex + 4; // 4 points of the triangle

    POINT topLeft = {start.x, start.y, start.z};
    POINT topRight = {start.x + side, start.y, start.z};
    POINT bottomRight = {start.x + side, start.y - side, start.z};
    POINT bottomLeft = {start.x, start.y - side, start.z};
    
    POINT backTopLeft = {start.x, start.y, start.z - 0.05f};
    POINT backTopRight = {start.x + side, start.y, start.z - 0.05f};
    POINT backBottomRight = {start.x + side, start.y - side, start.z - 0.05f};
    POINT backBottomLeft = {start.x, start.y - side, start.z - 0.05f};
    
    // Front face
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

    // Back face
    push_back(vector, &backTopLeft);
    push_back(indices, currentIndex);
    (*currentIndex)++;

    push_back(vector, &backTopRight);
    push_back(indices, currentIndex);
    push_back(indices, currentIndex);
    (*currentIndex)++;

    push_back(vector, &backBottomRight);
    push_back(indices, currentIndex);
    push_back(indices, currentIndex);
    (*currentIndex)++;

    push_back(vector, &backBottomLeft);
    push_back(indices, currentIndex);
    push_back(indices, currentIndex);
    push_back(indices, &backStartIndex);
    (*currentIndex)++;

    for (int i = 0 ; i < 4; i++)
    {
        GLuint connectionIndex = startIndex + i;
        GLuint backConnectionIndex = backStartIndex + i;

        push_back(indices, &connectionIndex);
        push_back(indices, &backConnectionIndex);
    }
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

        POINT topLeft = {start.x, start.y, start.z};
        POINT topRight = {start.x + 2 * side, start.y, start.z};
        POINT center = {start.x + side, start.y - side, start.z};
        POINT bottomLeft = {start.x, start.y - 2 * side, start.z};
        POINT bottomRight = {start.x + 2 * side, start.y - 2 * side, start.z};

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
    printf("%f ", point->y);
    printf("%f\n", point->z);
}