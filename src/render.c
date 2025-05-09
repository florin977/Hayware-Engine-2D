#include "utils.h"
#include "render.h"

// Function unbinds VBO after creation
GLuint createVBO(const VECTOR *vertices, const GLenum drawType)
{
    GLuint VBO = 0;

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices->currentIndex * vertices->dataSize, vertices->array, drawType);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return VBO;
}

GLuint createEBO(const VECTOR *indices, const GLenum drawType)
{
    GLuint EBO = 0;

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices->currentIndex * indices->dataSize, indices->array, drawType);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return EBO;
}

// VAO, VBO and EBO  unbound after function call; Pass 0 to EBO to not use it
GLuint createVAO(const GLuint VBO, const VECTOR *vertices, const GLuint EBO)
{
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    if (EBO != 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, vertices->dataSize, (void *)0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, vertices->dataSize, (void *)sizeof(POINT));
    glEnableVertexAttribArray(1);

    // Texture
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, vertices->dataSize, (void *)(sizeof(POINT) + sizeof(COLOR)));
    glEnableVertexAttribArray(2);

    // Unbind all buffers
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return VAO;
}

GLuint createTexture(const char *filename, const GLint internalFormat)
{
    stbi_set_flip_vertically_on_load(true);

    GLubyte *data = NULL;
    GLint textureWidth = DEFAULT_TEXTURE_WIDTH;
    GLint textureHeight = DEFAULT_TEXTURE_HEIGHT;
    GLint textureChannels = 0;

    GLuint texture = 0;

    if (filename[0] != 0)
    {
        data = stbi_load(filename, &textureWidth, &textureHeight, &textureChannels, 0);
    }

    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); // This linewas the reason for a lot of shader errors. It was GL_LINEAR_MIPMAP_NEAREST before.
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    if (filename[0] != 0)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, textureWidth, textureHeight, 0, internalFormat, GL_UNSIGNED_BYTE, data);
    }
    else 
    {
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, textureWidth, textureHeight, 0, internalFormat, GL_UNSIGNED_BYTE, 0);
    }
    
    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(data);

    return texture;
}

GLuint createFBO(const GLuint texture)
{
    GLuint FBO = 0;

    glGenFramebuffers(1, &FBO);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glBindTexture(GL_TEXTURE_2D, texture);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texture, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glBindTexture(GL_TEXTURE_2D, 0);

    return FBO;
}

GLuint compileShader(const char *filename, Shader_Type shaderType)
{
    FILE *in = NULL;

    if ((in = fopen(filename, "r")) == NULL)
    {
        perror(filename);
        exit(-1);
    }

    GLuint64 size = 0;

    fseek(in, 0, SEEK_END);

    size = ftell(in);

    fseek(in, 0, SEEK_SET);

    GLchar *shaderSource = NULL;

    if ((shaderSource = malloc((size + 1) * sizeof(GLchar))) == NULL)
    {
        fclose(in);
        perror("Shader malloc: ");
        exit(-1);
    }

    if (fread(shaderSource, sizeof(GLchar), size, in) != size * sizeof(GLchar))
    {
        free(shaderSource);
        fprintf(stderr, "Error reading from shader file: %s\n", filename);
        exit(-1);
    }

    shaderSource[size] = 0;

    if (fclose(in) != 0)
    {
        free(shaderSource);
        perror(filename);
        exit(-1);
    }

    GLuint shader = 0;

    switch (shaderType)
    {
    case Vertex_Shader:
    {
        shader = glCreateShader(GL_VERTEX_SHADER);
        break;
    }
    case Fragment_Shader:
    {
        shader = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    }
    }

    glShaderSource(shader, 1, (const GLchar *const *)&shaderSource, NULL);
    glCompileShader(shader);

    int success = 0;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
        free(shaderSource);
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        fprintf(stderr, "Shader compilation failed: %s\n", infoLog);
        exit(-1);
    }

    free(shaderSource);

    return shader;
}

GLuint createProgram(GLuint vertexShader, GLuint fragmentShader)
{
    GLuint shaderProgram = 0;

    shaderProgram = glCreateProgram();

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success = 0;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);

    if (!success)
    {
        glGetShaderInfoLog(shaderProgram, 512, NULL, infoLog);
        fprintf(stderr, "Shader Program linking failed: %s\n", infoLog);
        exit(-1);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return shaderProgram;
}