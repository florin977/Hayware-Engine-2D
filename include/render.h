#ifndef RENDER_H

#define RENDER_H

#include "init_window.h"
#include "cglm/cglm.h"
#include "stb_image.h"
#include "utils.h"

#define DEFAULT_TEXTURE_WIDTH 1000
#define DEFAULT_TEXTURE_HEIGHT 800

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

GLuint createTexture(const char *filename, const GLint internalFormat, const GLint format, const GLint channelSize);

GLuint createFBO(const GLuint texture);

GLuint createBreakoutFBO(const BREAKOUT_TEXTURE texture);

void bindBreakoutTextures(const BREAKOUT_TEXTURE texture, const GLuint shaderProgram);

GLuint compileShader(const char* filename, Shader_Type shaderType);

GLuint createProgram(GLuint vertexShader, GLuint fragmentShader);

BREAKOUT_TEXTURE createBreakoutTexture();

#endif