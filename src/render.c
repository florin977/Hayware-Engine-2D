#include "render.h"

// Function unbinds VBO after creation
GLuint create_VBO(const GLuint64 size, const VERTEX* vertices, const GLenum drawType)
{
    GLuint VBO = 0;

    glGenBuffers(1, &VBO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, size, vertices, drawType);

    glBindBuffer(GL_ARRAY_BUFFER, 0);

    return VBO;
}

GLuint create_EBO(const GLuint64 size, const GLuint* indices, const GLenum drawType)
{
    GLuint EBO = 0;

    glGenBuffers(1, &EBO);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, drawType);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return EBO;
}

// VAO, VBO and EBO  unbound after function call; Pass 0 to EBO to not use it
GLuint create_VAO(const GLuint VBO, const GLuint EBO)
{
    GLuint VAO = 0;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    if (EBO != 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    }
    else 
    {
        //fprintf(stderr, "No EBO\n");
        //exit(-1);
    }

    // Position
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*) 0);
    glEnableVertexAttribArray(0);

    // Color
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);

    // Texture coordinates
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VERTEX), (void*)(6 * sizeof(GLfloat)));
    glEnableVertexAttribArray(2);

    glBindVertexArray(0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    return VAO;
}

GLuint compile_shader(const char* filename, Shader_Type shaderType)
{
    FILE* in = NULL;

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

    switch(shaderType)
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

    glShaderSource(shader, 1, (const GLchar* const*) &shaderSource, NULL);
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

GLuint create_program(GLuint vertexShader, GLuint fragmentShader)
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