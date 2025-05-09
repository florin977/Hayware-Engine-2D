#version 460 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aColor;
layout (location = 2) in vec2 aTextureCoord;

out vec3 outColor;
out vec2 outTextureCoord;

void main()
{
    float scaleFactor = 10.0;
    mat3 scaleMatrix = mat3(scaleFactor,  0.0,          0.0,
                            0.0,       scaleFactor,     0.0,
                            0.0,          0.0,      scaleFactor);

    gl_Position = vec4(scaleMatrix * aPos, 1.0);

    outColor = aColor;

    outTextureCoord = aTextureCoord;
}

// Yellow: vec4(0.76, 0.73, 0.0, 1.0)
// Dark-Green: vec4(0.0f, 0.25f, 0.23f, 1.0f)