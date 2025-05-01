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

        //SDL_GL_SetSwapInterval(0); // Disable VSYNC
        
        GLuint gameTexture = createTexture("../textures/Daerian.png", GL_RGB);

        GLuint texture[2];
        texture[0] = createTexture("", GL_RGBA);
        texture[1] = createTexture("", GL_RGBA);

        GLuint FBO[2];
        FBO[0] = createFBO(texture[0]);
        FBO[1] = createFBO(texture[1]);

        GLboolean pingPong = true;

        GLubyte *pixels = malloc(4 * 1000 * 800); // RGBA = 4 bytes

        while (running)
        {
            glDisable(GL_DEPTH_TEST);

            glUseProgram(shaderProgram);
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

                        glUniform2f(glGetUniformLocation(shaderProgram, "mouseCoord"), mouseX, mouseY);
                        glUniform1i(glGetUniformLocation(shaderProgram, "mousePressed"), 1);

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
            
            GLuint64 currentTime = SDL_GetTicks();

            GLuint readTexture;
            GLuint displayTexture;

            if (currentTime - time >= 0)
            {
                time = currentTime;

                if (pingPong)
                {
                    readTexture = texture[0];
                    displayTexture = texture[1];
                    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO[1]);
                }
                else 
                {
                    readTexture = texture[1];
                    displayTexture = texture[0];
                    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, FBO[0]);
                }
            }

            pingPong = !pingPong;
            // Render to texture
            glBindTexture(GL_TEXTURE_2D, readTexture);
            
            glBindVertexArray(VAOquad);
            glBindBuffer(GL_BUFFER, VBOquad);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOquad);

            glDrawElements(GL_TRIANGLES, indicesQuad.currentIndex, GL_UNSIGNED_INT, 0);
            
            // Unbind
            glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);
            glBindTexture(GL_TEXTURE_2D, 0);

            glClearColor(0.0f, 0.30f, 0.30f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT); // Clear screen

            // Render to screen
            glBindVertexArray(VAOquad);
            glBindBuffer(GL_BUFFER, VBOquad);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOquad);
            glUseProgram(shaderProgramQuad);

            glBindTexture(GL_TEXTURE_2D, displayTexture);
            
            glDrawElements(GL_TRIANGLES, indicesQuad.currentIndex, GL_UNSIGNED_INT, 0);

            // Draw to window
            SDL_GL_SwapWindow(window);
        }

        quitSDL(window);

        free(vertices.array);
        free(indices.array);
    }