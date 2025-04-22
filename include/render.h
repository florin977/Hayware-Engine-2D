#ifndef RENDER_H

#define RENDER_H

#include "init_window.h"
#include "cglm/cglm.h"
#include "stb_image.h"

typedef struct
{
    // Position
    GLfloat x;
    GLfloat y;
    GLfloat z;

    // Color
    GLfloat r;
    GLfloat g;
    GLfloat b;

    // Texture coordinates
    GLfloat s;
    GLfloat t;
} VERTEX;

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

GLuint create_VBO(const GLuint64 size, const VERTEX* vertices, const GLenum drawType);

GLuint create_EBO(const GLuint64 size, const GLuint* indices, const GLenum drawType);

GLuint create_VAO(const GLuint VBO, const GLuint EBO);

GLuint compile_shader(const char* filename, Shader_Type shaderType);

GLuint create_program(GLuint vertexShader, GLuint fragmentShader);

GLuint create_texture(const char* filepath, const GLenum textureFormat);

#endif