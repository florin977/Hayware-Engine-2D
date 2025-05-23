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

    window = init_window(DEFAULT_TEXTURE_WIDTH, DEFAULT_TEXTURE_HEIGHT, "Hayware Engine");

    SDL_GLContext glContext = init_glContext(window);

    SDL_Event event;
    int8_t running = 1;

    // Creating the quad covering the screen
    VECTOR verticesQuad = createVector(sizeof(VERTEX)); // Vertex data
    VECTOR indicesQuad = createVector(sizeof(GLuint));  // Index data

    createFullscreenQuad(&verticesQuad, &indicesQuad);

    // Buffers for the quad
    GLuint VBOquad = createVBO(&verticesQuad, GL_STATIC_DRAW);
    GLuint EBOquad = createEBO(&indicesQuad, GL_STATIC_DRAW);
    GLuint VAOquad = createVAO(VBOquad, &verticesQuad, EBOquad);

    // Shaders

    // Used for the game logic (drawing pixels on the texture)
    GLuint vertexShader = compileShader("/home/andrei/1HaywareEngine/src/shaders/vertexShader.vert", Vertex_Shader);
    GLuint fragmentShader = compileShader("/home/andrei/1HaywareEngine/src/shaders/fragShader.frag", Fragment_Shader);
    // -------

    // Used to display the rendered texture to the screen
    GLuint vertexShaderQuad = compileShader("/home/andrei/1HaywareEngine/src/shaders/vertexShaderQuad.vert", Vertex_Shader);
    GLuint fragmentShaderQuad = compileShader("/home/andrei/1HaywareEngine/src/shaders/fragShaderQuad.frag", Fragment_Shader);
    // -------

    // Shader programs for the above shaders
    GLuint shaderProgram = createProgram(vertexShader, fragmentShader);
    GLuint shaderProgramQuad = createProgram(vertexShaderQuad, fragmentShaderQuad);
    // -------

    // SDL_GL_SetSwapInterval(0); // Uncomment to disable VSYNC

    // Create textures and FBOs
    BREAKOUT_TEXTURE FBOtexture[2] = {createBreakoutTexture(), createBreakoutTexture()};

    GLuint FBO[2] = {createBreakoutFBO(FBOtexture[0]), createBreakoutFBO(FBOtexture[1])};
    // -------

    // Boolean to be used for ping ponging between the FBOs
    GLboolean pingPong = true;

    GLint passUniform = glGetUniformLocation(shaderProgram, "currentPass");
    GLint pass = 0;

    GLboolean mouseIsPressed = false;
    GLboolean mouseWasMoved = false;
    GLint mouseX = 0;
    GLint mouseY = 0;

    while (running)
    {
        glDisable(GL_DEPTH_TEST);

        glUseProgram(shaderProgram);

        glUniform1i(passUniform, pass);

        if (pass < 3)
        {
            pass++;
        }
        else 
        {
            pass = 0;
        }

        // Handle user interactions
        while (SDL_PollEvent(&event))
        {
            // Mouse's position changed, do not send info in the main loop
            mouseWasMoved = true;
            
            switch (event.type)
            {
            case SDL_EVENT_QUIT:
                running = 0;
                break;

            case SDL_EVENT_MOUSE_BUTTON_DOWN:
                mouseX = event.button.x;
                mouseY = DEFAULT_TEXTURE_HEIGHT - event.button.y; // Make Y go from 0 (bottom) to 800 (top)
                mouseIsPressed = true;

                printf("%d %d\n", mouseX, mouseY);

                // Send mouse position to the shader
                glUniform2i(glGetUniformLocation(shaderProgram, "mouseCoord"), mouseX, mouseY);
                glUniform1i(glGetUniformLocation(shaderProgram, "mousePressed"), 1); // Mouse is pressed
                break;

            case SDL_EVENT_MOUSE_BUTTON_UP:
                mouseIsPressed = false;
                glUniform1i(glGetUniformLocation(shaderProgram, "mousePressed"), 0); // Mouse is no longer pressed
                break;

            case SDL_EVENT_KEY_DOWN:
                // Exit the app
                if (event.key.key == SDLK_ESCAPE)
                {
                    running = 0;
                }
                break;
            }

            // Only send the new position, do not change the uniform 'mousePressed'
            if (mouseIsPressed)
            {
                mouseX = event.button.x;
                mouseY = DEFAULT_TEXTURE_HEIGHT - event.button.y;

                // Send mouse position to the shader
                glUniform2i(glGetUniformLocation(shaderProgram, "mouseCoord"), mouseX, mouseY);
            }
        }

        // Mouse is being held and the position has not changed (allow sand to fall from a single point)
        if (!mouseWasMoved && mouseIsPressed)
        {
            // Send mouse position to the shader
            glUniform2i(glGetUniformLocation(shaderProgram, "mouseCoord"), mouseX, mouseY);
        }

        mouseWasMoved = false;

        GLuint64 currentTime = SDL_GetTicks();

        BREAKOUT_TEXTURE readTexture;
        BREAKOUT_TEXTURE displayTexture;

        if (pingPong)
        {
            readTexture = FBOtexture[0];
            displayTexture = FBOtexture[1];
            
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO[1]);
        }
        else
        {
            readTexture = FBOtexture[1];
            displayTexture = FBOtexture[0];

            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO[0]);
        }

        pingPong = !pingPong;

        // Render to texture

        // Read from readTexture, but write to the texture in the other FBO

        bindBreakoutTextures(readTexture, shaderProgram);

        glBindVertexArray(VAOquad);
        glBindBuffer(GL_BUFFER, VBOquad);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOquad);

        glDrawElements(GL_TRIANGLES, indicesQuad.currentIndex, GL_UNSIGNED_INT, 0);

        // Unbind FBO and texture
        glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
        glBindTexture(GL_TEXTURE_2D, 0);

        // Clear the screen with dark-teal color
        glClearColor(0.0f, 0.30f, 0.30f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Render to the screen
        glBindVertexArray(VAOquad);
        glBindBuffer(GL_BUFFER, VBOquad);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOquad);
        glUseProgram(shaderProgramQuad);

        bindBreakoutTextures(displayTexture, shaderProgramQuad);

        glDrawElements(GL_TRIANGLES, indicesQuad.currentIndex, GL_UNSIGNED_INT, 0);

        // Draw
        SDL_GL_SwapWindow(window);

        SDL_Delay(0);
    }

    quitSDL(window);

    free(verticesQuad.array);
    free(indicesQuad.array);
}