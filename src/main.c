#include "init_window.h"
#include "render.h"
#include "utils.h"

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

    VERTEX *vertices = NULL;
    GLuint *indices = NULL;

    GLuint64 verticesSize = 0;
    GLuint64 indicesSize = 0;

    GLuint VBO = 0;

    VERTEX start = { -0.25f, 0.25f, 0, 255, 255, 255, 0, 0 };
    
    glEnable(GL_LINE_SMOOTH);
    glLineWidth(2.0f); // Make lines thicker

    VBO = create_VBO(verticesSize, vertices, GL_STATIC_DRAW);

    GLuint EBO = 0;

    EBO = create_EBO(indicesSize, indices, GL_STATIC_DRAW);

    GLuint vertexShader = 0;
    GLuint fragmentShader = 0;

    vertexShader = compile_shader("/home/andrei/1HaywareEngine/src/shaders/vertexShader.vert", Vertex_Shader);
    fragmentShader = compile_shader("/home/andrei/1HaywareEngine/src/shaders/fragShader.frag", Fragment_Shader);

    GLuint shaderProgram = 0;
    shaderProgram = create_program(vertexShader, fragmentShader);

    GLuint VAO = 0;
    VAO = create_VAO(VBO, EBO);

    GLuint64 time = SDL_GetTicks();
    GLuint64 frames = 0;

    SDL_GL_SetSwapInterval(0); // Disable VSYNC

    VECTOR v = createVector(sizeof(GLuint));
    GLuint a = 10;
        
    push_back2(&v, &a);
    push_back2(&v, &a);
    push_back2(&v, &a);
    push_back2(&v, &a);
    push_back2(&v, &a);
    push_back2(&v, &a);
    push_back2(&v, &a);
    push_back2(&v, &a);
    push_back2(&v, &a);
    push_back2(&v, &a);

    for (int i = 0; i < v.currentIndex; i++)
    {
        printf("VECTOR: %d\n", i);
    }

    while (running)
    {
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

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
            printf("FPS: %lu\n", frames);
            frames = 0;
            time = currentTime;
        }

        // Use shaders and VAO
 
        glUseProgram(shaderProgram);

        glBindVertexArray(VAO);

        glDrawElements(GL_LINES, indicesSize, GL_UNSIGNED_INT, 0);

        SDL_GL_SwapWindow(window);
    }

    quitSDL(window);
}