#version 460 core

in vec3 outColor;

in vec2 outTextureCoord;

uniform sampler2D outTexture;
uniform vec2 mouseCoord;
uniform bool mousePressed;

out vec4 FragColor;

void main()
{   
    /*
    if (mousePressed)
    {
        FragColor = vec4(0.0, 1.0, 0.0, 1.0);
    }
    else 
    {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    }
    */
    FragColor = texture(outTexture, outTextureCoord);

    if (mousePressed && distance(outTextureCoord, mouseCoord) <= 0.001)
    {
        FragColor = vec4(1.0, 1.0, 1.0, 1.0);
    }
}