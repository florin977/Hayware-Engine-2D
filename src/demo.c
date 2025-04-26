#include "demo.h"

void drawSquareFullFace(VECTOR *vertices, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex)
{
    // Indices
    GLuint topLeftIndex = *currentIndex;
    GLuint backTopLeftIndex = topLeftIndex + SQUARE_POINTS; // 4 points of the square

    GLuint topRightIndex = *currentIndex + 1;
    GLuint backTopRightIndex = topRightIndex + SQUARE_POINTS;

    GLuint bottomRightIndex = *currentIndex + 2;
    GLuint backBottomRightIndex = bottomRightIndex + SQUARE_POINTS;

    GLuint bottomLeftIndex = *currentIndex + 3;
    GLuint backBottomLeftIndex = bottomLeftIndex + SQUARE_POINTS;

    *currentIndex = backBottomLeftIndex + 1;

    COLOR frontCol = {0.0f, 0.0f, 1.0f};
    COLOR backCol = {1.0f, 0.0f, 0.0f};
    // Vertices
    VERTEX topLeft = createVertex((POINT){start.x, start.y, start.z}, frontCol, (TEXTURE){0.0f, 0.0f});
    VERTEX topRight = createVertex((POINT){start.x + side, start.y, start.z}, frontCol, (TEXTURE){0.0f, 0.0f});
    VERTEX bottomRight = createVertex((POINT){start.x + side, start.y - side, start.z}, frontCol, (TEXTURE){0.0f, 0.0f});
    VERTEX bottomLeft = createVertex((POINT){start.x, start.y - side, start.z}, frontCol, (TEXTURE){0.0f, 0.0f});
    
    VERTEX backTopLeft = createVertex((POINT){start.x, start.y, start.z + 0.05f}, backCol, (TEXTURE){0.0f, 0.0f});
    VERTEX backTopRight = createVertex((POINT){start.x + side, start.y, start.z + 0.05f}, backCol, (TEXTURE){0.0f, 0.0f});
    VERTEX backBottomRight = createVertex((POINT){start.x + side, start.y - side, start.z + 0.05f}, backCol, (TEXTURE){0.0f, 0.0f});
    VERTEX backBottomLeft = createVertex((POINT){start.x, start.y - side, start.z + 0.05f}, backCol, (TEXTURE){0.0f, 0.0f});
    
    //Vertices vector
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

    // Indices vector
    // Front face
    push_back(indices, &topLeftIndex);
    push_back(indices, &topRightIndex);
    push_back(indices, &bottomRightIndex);

    push_back(indices, &topLeftIndex);
    push_back(indices, &bottomRightIndex);
    push_back(indices, &bottomLeftIndex);

    // Back face
    push_back(indices, &backTopLeftIndex);
    push_back(indices, &backTopRightIndex);
    push_back(indices, &backBottomRightIndex);

    push_back(indices, &backTopLeftIndex);
    push_back(indices, &backBottomRightIndex);
    push_back(indices, &backBottomLeftIndex);

    // Connections

    // Top
    push_back(indices, &topLeftIndex);
    push_back(indices, &backTopLeftIndex);
    push_back(indices, &backTopRightIndex);

    push_back(indices, &topLeftIndex);
    push_back(indices, &backTopRightIndex);
    push_back(indices, &topRightIndex);

    // Right
    push_back(indices, &topRightIndex);
    push_back(indices, &backTopRightIndex);
    push_back(indices, &backBottomRightIndex);

    push_back(indices, &topRightIndex);
    push_back(indices, &backBottomRightIndex);
    push_back(indices, &bottomRightIndex);

    //Bottom
    push_back(indices, &bottomLeftIndex);
    push_back(indices, &backBottomLeftIndex);
    push_back(indices, &backBottomRightIndex);

    push_back(indices, &bottomLeftIndex);
    push_back(indices, &backBottomRightIndex);
    push_back(indices, &bottomRightIndex);

    // Left
    push_back(indices, &bottomLeftIndex);
    push_back(indices, &backBottomLeftIndex);
    push_back(indices, &backTopLeftIndex);

    push_back(indices, &bottomLeftIndex);
    push_back(indices, &backTopLeftIndex);
    push_back(indices, &topLeftIndex);
}

void drawSquare(VECTOR *vertices, VECTOR *indices, POINT start, GLfloat side, GLuint *currentIndex)
{
    // Indices
    GLuint startIndex = *currentIndex;
    GLuint backStartIndex = startIndex + SQUARE_POINTS; // 4 points of the square

    // Vertices
    POINT topLeft = {start.x, start.y, start.z};
    POINT topRight = {start.x + side, start.y, start.z};
    POINT bottomRight = {start.x + side, start.y - side, start.z};
    POINT bottomLeft = {start.x, start.y - side, start.z};
    
    POINT backTopLeft = {start.x, start.y, start.z - 0.05f};
    POINT backTopRight = {start.x + side, start.y, start.z - 0.05f};
    POINT backBottomRight = {start.x + side, start.y - side, start.z - 0.05f};
    POINT backBottomLeft = {start.x, start.y - side, start.z - 0.05f};
    
    //Vertices vector

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

    // Indices vector
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
        //drawSquare(vertices, indices, start, side, currentIndex);
        drawSquareFullFace(vertices, indices, start, side, currentIndex);
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