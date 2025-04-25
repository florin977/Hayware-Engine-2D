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

void push_back_old(void **array, GLuint64 *size, GLuint64 *position, void *element, uint8_t dataSize)
{
    if (*position == *size)
    {
        *size = *size + CHUNK;

        void *temp = NULL;

        if ((temp = realloc(*array,  *size * dataSize)) == NULL)
        {
            free(*array);
            perror("Realloc:");
            exit(EXIT_FAILURE);
        }

        *array = temp;
    }

    memcpy((char*)(*array) + (*position * dataSize), element, dataSize);
    (*position)++;
}