#include "init_window.h"

SDL_Window *init_window(const uint32_t width, const uint32_t height, const char *window_name)
{
    SDL_Window *window = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) == 0)
    {
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(-1);
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    // Modify for double buffering!

    window = SDL_CreateWindow("Hayware Engine", width, height, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

    if (window == NULL)
    {
        SDL_Quit();
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(-1);
    }

    return window;
}

SDL_GLContext init_glContext(SDL_Window *window)
{
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    if (glContext == NULL)
    {
        SDL_DestroyWindow(window);
        SDL_Quit();
        fprintf(stderr, "%s\n", SDL_GetError());
        exit(-1);
    }

    if (glewInit() != GLEW_OK)
    {
        fprintf(stderr, "Error initialising GLEW\n");
        exit(-1);
    }
    
    return glContext;
}

void quitSDL(SDL_Window *window)
{   
    SDL_DestroyWindow(window);
    SDL_Quit();
}