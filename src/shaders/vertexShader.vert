#version 460 core
layout (location = 0) in vec2 aPos;


void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);
}

// Yellow: vec4(0.76, 0.73, 0.0, 1.0)
// Dark-Green: vec4(0.0f, 0.25f, 0.23f, 1.0f)