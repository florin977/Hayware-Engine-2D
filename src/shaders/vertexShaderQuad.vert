#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoord;

out vec3 outColor;
out vec2 outTextureCoord;

void main()
{

    gl_Position = vec4(aPos, 1.0);

    outColor = aColor;

    outTextureCoord = aTextureCoord;
}

// Yellow: vec4(0.76, 0.73, 0.0, 1.0)
// Dark-Green: vec4(0.0f, 0.25f, 0.23f, 1.0f)