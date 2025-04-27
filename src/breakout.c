#include "breakout.h"

CANVAS createCanvas()
{
    COLOR canvasColor = {1.0f, 1.0f, 1.0f};
    TEXTURE canvasTexture = {0.0f, 0.0f};

    VERTEX topLeft = {{-1.0f, 1.0f, 0.1f}, canvasColor, canvasTexture};
    VERTEX topRight = {{1.0f, 1.0f, 0.1f}, canvasColor, canvasTexture};
    VERTEX bottomRight = {{1.0f, -1.0f, 0.1f}, canvasColor, canvasTexture};
    VERTEX bottomLeft = {{-1.0f, -1.0f, 0.1f}, canvasColor, canvasTexture};

    GLuint index = 0;
    VECTOR vertices = createVector(sizeof(VERTEX));
    VECTOR indices = createVector(sizeof(GLuint));

    CANVAS canvas = {vertices, indices};

    push_back(&canvas.vertices, &topLeft);
    push_back(&canvas.vertices, &topRight);
    push_back(&canvas.vertices, &bottomRight);
    push_back(&canvas.vertices, &bottomLeft);

    index = 0;
    push_back(&canvas.indices, &index);
    index = 1;
    push_back(&canvas.indices, &index);
    index = 2;
    push_back(&canvas.indices, &index);
    push_back(&canvas.indices, &index);
    index = 0;
    push_back(&canvas.indices, &index);
    index = 3;
    push_back(&canvas.indices, &index);

    return canvas;
}