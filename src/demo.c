#include "demo.h"

void drawSquare(VECTOR *vertices, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex)
{
    GLuint startIndex = *currentIndex;
    GLuint backStartIndex = startIndex + SQUARE_POINTS; // 4 points of the square

    POINT topLeft = {start.x, start.y, start.z};
    POINT topRight = {start.x + side, start.y, start.z};
    POINT bottomRight = {start.x + side, start.y - side, start.z};
    POINT bottomLeft = {start.x, start.y - side, start.z};
    
    POINT backTopLeft = {start.x, start.y, start.z - 0.05f};
    POINT backTopRight = {start.x + side, start.y, start.z - 0.05f};
    POINT backBottomRight = {start.x + side, start.y - side, start.z - 0.05f};
    POINT backBottomLeft = {start.x, start.y - side, start.z - 0.05f};
    
    //Vertices 

    // Front face
    push_back(vertices, &topLeft);
    push_back(vertices, &topRight);
    push_back(vertices, &bottomRight);
    push_back(vertices, &bottomLeft);

    // Back face
    push_back(vertices, &backTopLeft);
    push_back(vertices, &backTopRight);
    push_back(vertices, &backBottomRight);
    push_back(vertices, &backBottomLeft);

    // Indices
    push_back(indices, currentIndex); // Start index (topLeft vertex)
    (*currentIndex)++;


    for (int i = 1; i < 2 * SQUARE_POINTS; i++)
    {
        if (i == SQUARE_POINTS)
        {
            // Return to topLeft vertex and then start from backTopLeft
            push_back(indices, &startIndex);
            
            push_back(indices, currentIndex);
            (*currentIndex)++;
            continue;
        }

        // Draw lines between squares (startIndex-startIndex + 1 == first line, startIndex + 1-startIndex + 2 == second line and so on)
        push_back(indices, currentIndex);
        push_back(indices, currentIndex);
        (*currentIndex)++;
    }

    // Return to backTopLeft and close off the drawing
    push_back(indices, &backStartIndex);

    for (int i = 0 ; i < SQUARE_POINTS; i++)
    {
        // Connect the front and back squares (offset between them == SQUARE_POINTS, which is 4)
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