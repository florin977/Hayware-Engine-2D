#include "utils.h"
#include "render.h"

VECTOR createVector(uint8_t dataSize)
{
    VECTOR v;
    v.array = NULL;
    v.currentIndex = 0;
    v.size = 0;
    v.dataSize = dataSize;

    return v;
}

VERTEX createVertex(POINT pos, COLOR col, TEXTURE tex)
{
    VERTEX v;

    v.position.x = pos.x;
    v.position.y = pos.y;
    v.position.z = pos.z;
    v.color.r = col.r;
    v.color.g = col.g;
    v.color.b = col.b;
    v.texture.s = tex.s;
    v.texture.t = tex.t;
    v.texture.r = tex.r;

    return v;
}

void createFullscreenQuad(VECTOR *vertices, VECTOR *indices)
{
    VERTEX topLeft = {{-1.0f, 1.0f, 0.1f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}};
    VERTEX topRight = {{1.0f, 1.0f, 0.1f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 0.0f}};
    VERTEX bottomRight = {{1.0f, -1.0f, 0.1f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}};
    VERTEX bottomLeft = {{-1.0f, -1.0f, 0.1f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 0.0f}};

    push_back(vertices, &topLeft);
    push_back(vertices, &topRight);
    push_back(vertices, &bottomRight);
    push_back(vertices, &bottomLeft);

    GLuint index = 0;

    index = 0; push_back(indices, &index);
    index = 1; push_back(indices, &index);
    index = 2; push_back(indices, &index);

    index = 0; push_back(indices, &index);
    index = 2; push_back(indices, &index);
    index = 3; push_back(indices, &index);
}

void push_back(VECTOR *v, void *element)
{
    if (v->currentIndex == v->size)
    {
        v->size = v->size + CHUNK;

        void *temp = NULL;

        if ((temp = realloc(v->array, v->size * v->dataSize)) == NULL)
        {
            free(v->array);
            perror("Realloc error:");
            exit(EXIT_FAILURE);
        }

        v->array = temp;
    }

    memcpy((char*)v->array + (v->currentIndex * v->dataSize), element, v->dataSize);
    (v->currentIndex)++;
}

void *getElement(VECTOR *v, GLuint64 index)
{
    if (index >= v->size)
    {
        return NULL;
    }

    return (char *)v->array + (index * v->dataSize);
}

GLdouble toRadians(GLdouble degrees)
{
    return (degrees * (PI / 180));
}