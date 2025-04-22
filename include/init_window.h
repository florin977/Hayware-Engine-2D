#ifndef INIT_WINDOW_H

#define INIT_WINDOW_H

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <stdint.h>
#include <time.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL3/SDL.h>
#include <SDL3/SDL_opengl.h>
#include <SDL3/SDL_video.h>

SDL_Window *init_window(const uint32_t width, const uint32_t height, const char *window_name);

void quitSDL(SDL_Window *window);

SDL_GLContext init_glContext(SDL_Window *window);

#endif