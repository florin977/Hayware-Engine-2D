#include "init_window.h"
#include "render.h"
#include "utils.h"
#include "breakout.h"
#include "demo.h"

int main(int argc, char **argv)
{
    srand(time(NULL));
    clock_t lastTime = SDL_GetTicks();
    clock_t currentTime;

    SDL_Window *window = NULL;

    window = init_window(1000, 800, "Hayware Engine");

    SDL_GLContext glContext = init_glContext(window);

    SDL_Event event;
    int8_t running = 1;

    VECTOR vertices = createVector(sizeof(VERTEX));
    VECTOR indices = createVector(sizeof(GLuint));

    VECTOR verticesQuad = createVector(sizeof(VERTEX));
    VECTOR indicesQuad = createVector(sizeof(GLuint));

    POINT start = {-0.5, 0.5, 0};
    GLfloat side = 1.0f;
    GLuint currentIndex = 0;

    boxFractal(&vertices, &indices, start, side, &currentIndex);

    createFullscreenQuad(&verticesQuad, &indicesQuad);

    GLuint VBO = createVBO(&vertices, GL_STATIC_DRAW);
    GLuint EBO = createEBO(&indices, GL_STATIC_DRAW);
    GLuint VAO = createVAO(VBO, &vertices, EBO);

    GLuint VBOquad = createVBO(&verticesQuad, GL_STATIC_DRAW);
    GLuint EBOquad = createEBO(&indicesQuad, GL_STATIC_DRAW);
    GLuint VAOquad = createVAO(VBOquad, &verticesQuad, EBOquad);

    GLuint vertexShader = compileShader("/home/andrei/1HaywareEngine/src/shaders/vertexShader.vert", Vertex_Shader);
    GLuint fragmentShader = compileShader("/home/andrei/1HaywareEngine/src/shaders/fragShader.frag", Fragment_Shader);

    GLuint vertexShaderQuad = compileShader("/home/andrei/1HaywareEngine/src/shaders/vertexShaderQuad.vert", Vertex_Shader);
    GLuint fragmentShaderQuad = compileShader("/home/andrei/1HaywareEngine/src/shaders/fragShaderQuad.frag", Fragment_Shader);

    GLuint shaderProgram = createProgram(vertexShader, fragmentShader);
    GLuint shaderProgramQuad = createProgram(vertexShaderQuad, fragmentShaderQuad);

    GLuint64 time = SDL_GetTicks();
    GLuint64 frames = 0;

    GLuint xRotationUniform = glGetUniformLocation(shaderProgram, "xAngle");
    GLuint zRotationUniform = glGetUniformLocation(shaderProgram, "zAngle");

    GLdouble xAngle = 0.0f;
    GLdouble zAngle = 0.0f;

    SDL_GL_SetSwapInterval(0); // Disable VSYNC
    
    GLuint gameTexture = createTexture("../textures/Daerian.png", GL_RGB);
    GLuint quadTexture = createTexture("", GL_RGBA);

    //GLuint texture[2] = {createTexture("", GL_RGBA)};
    //GLuint FBO[2] = {createFBO(texture[0]), createFBO(texture[1])};

    GLuint FBO = createFBO(quadTexture);

    while (running)
    {
        // Handle user interactions
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_EVENT_QUIT:
                    running = 0;
                    break;
                case SDL_EVENT_MOUSE_BUTTON_DOWN:
                    GLfloat mouseX = event.button.x / 1000;
                    GLfloat mouseY = 1.0f - event.button.y / 800;

                    glUniform2f(glGetUniformLocation(shaderProgramQuad, "mouseCoord"), mouseX, mouseY);
                    glUniform1i(glGetUniformLocation(shaderProgramQuad, "mousePressed"), 1);

                    printf("%f %f\n", mouseX, mouseY);
                    break;
                case SDL_EVENT_MOUSE_BUTTON_UP:
                    glUniform1i(glGetUniformLocation(shaderProgram, "mousePressed"), 0);
                    break;
                case SDL_EVENT_KEY_DOWN:
                    if (event.key.scancode == SDL_SCANCODE_W)
                    {
                        xAngle += 5.0f;
                    }
                    if (event.key.scancode == SDL_SCANCODE_S)
                    {
                        xAngle -= 5.0f;
                    }
                    if (event.key.scancode == SDL_SCANCODE_A)
                    {
                        zAngle += 5.0f;
                    }
                    if (event.key.scancode == SDL_SCANCODE_D)
                    {
                        zAngle -= 5.0f;
                    }
                    if (event.key.key == SDLK_ESCAPE)
                    {
                        running = 0;
                    }
                    break;

            }
        }
        
        GLdouble xAngleInRadians = toRadians(xAngle); 
        GLdouble zAngleInRadians = toRadians(zAngle);

        // Calculate FPS
        GLuint64 currentTime = SDL_GetTicks();
        frames++;

        if (currentTime - time >= 1000)
        {
            printf("FPS: %lu\n", frames);
            frames = 0;
            time = currentTime;
        }

        // 1 ---------------------------------------------
        glBindFramebuffer(GL_FRAMEBUFFER, FBO);
        glEnable(GL_DEPTH_TEST);

        glBindTexture(GL_TEXTURE_2D, gameTexture);

        // Clear screen with dark-teal
        glClearColor(0.0f, 0.30f, 0.30f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer
        
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glUseProgram(shaderProgram);

        glUniform1f(xRotationUniform, xAngleInRadians);
        glUniform1f(zRotationUniform, zAngleInRadians);

        glDrawElements(GL_TRIANGLES, indices.currentIndex, GL_UNSIGNED_INT, 0);

        // 2 ------------------------------------------------

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glDisable(GL_DEPTH_TEST);

        glBindTexture(GL_TEXTURE_2D, quadTexture);

        // Clear screen with dark-teal
        glClearColor(0.0f, 0.30f, 0.30f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear screen and depth buffer
        
        glBindVertexArray(VAOquad);
        glBindBuffer(GL_ARRAY_BUFFER, VBOquad);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOquad);

        glUseProgram(shaderProgramQuad);

        glDrawElements(GL_TRIANGLES, indicesQuad.currentIndex, GL_UNSIGNED_INT, 0);

        // ------------------------

        SDL_GL_SwapWindow(window);
    }

    quitSDL(window);

    free(vertices.array);
    free(indices.array);
}