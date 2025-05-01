#version 460 core

in vec3 outColor;

in vec2 outTextureCoord;

uniform sampler2D outTexture;
uniform vec2 mouseCoord;
uniform bool mousePressed;

out vec4 FragColor;

void main()
{   
    FragColor = texture(outTexture, outTextureCoord);
}