#include "init_window.h"
#include "render.h"
#include "utils.h"
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

    VECTOR vertices = createVector(sizeof(POINT));
    VECTOR indices = createVector(sizeof(GLuint));

    POINT start = {-1, 1};
    GLfloat side = 2.0f;
    GLuint depth = 5;
    GLuint currentIndex = 0;

    boxFractal(&vertices, &indices, start, side, &currentIndex, depth);

    GLuint VBO = 0;
    GLuint EBO = 0;
    GLuint VAO = 0;

    VBO = createVBO(&vertices, GL_STATIC_DRAW);
    EBO = createEBO(&indices, GL_STATIC_DRAW);
    VAO = createVAO(VBO, &vertices, EBO);

    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f); // Make lines thicker

    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;

    vertexShader = compileShader("/home/andrei/1HaywareEngine/src/shaders/vertexShader.vert", Vertex_Shader);
    fragmentShader = compileShader("/home/andrei/1HaywareEngine/src/shaders/fragShader.frag", Fragment_Shader);

    GLuint shaderProgram = 0;
    shaderProgram = createProgram(vertexShader, fragmentShader);

    GLuint64 time = SDL_GetTicks();
    GLuint64 frames = 0;

    SDL_GL_SetSwapInterval(0); // Disable VSYNC

    while (running)
    {
        // Handle user interactions
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_EVENT_QUIT)
            {
                running = 0;
            }
        }

        // Clear screen with dark-teal
        glClearColor(0.0f, 0.30f, 0.30f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Calculate FPS
        GLuint64 currentTime = SDL_GetTicks();
        frames++;

        if (currentTime - time >= 1000)
        {
            // printf("FPS: %lu\n", frames);
            frames = 0;
            time = currentTime;
        }

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        glUseProgram(shaderProgram);

        glDrawElements(GL_LINES, indices.currentIndex, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    quitSDL(window);
}