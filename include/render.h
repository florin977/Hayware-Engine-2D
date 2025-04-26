#ifndef RENDER_H

#define RENDER_H

#include "init_window.h"
#include "cglm/cglm.h"
#include "stb_image.h"
#include "utils.h"

typedef struct
{
    GLuint x;
    GLuint y;
    GLuint Z;
} Indices;

typedef enum
{
    Vertex_Shader,
    Fragment_Shader,
} Shader_Type;

GLuint createVBO(const VECTOR *vertices, const GLenum drawType);

GLuint createEBO(const VECTOR *indices, const GLenum drawType);

GLuint createVAO(const GLuint VBO, const VECTOR *vertices, const GLuint EBO);

GLuint compileShader(const char* filename, Shader_Type shaderType);

GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);

GLuint createTexture(const char* filepath, const GLenum textureFormat);

#endif