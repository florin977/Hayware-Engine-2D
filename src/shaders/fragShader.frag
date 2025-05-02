#version 460 core

in vec3 outColor;

in vec2 outTextureCoord;

uniform sampler2D outTexture;
uniform vec2 mouseCoord;
uniform bool mousePressed;

out vec4 FragColor;

#define TOLERANCE 0.005
#define MOUSE_PRESS_TOLERANCE 0.005
#define TEXEL_SIZE_X (1.0 / 1000) 
#define TEXEL_SIZE_Y (1.0 / 800)
#define mouseEquals(a, b) (distance(a, b) <= MOUSE_PRESS_TOLERANCE)
#define equals(a, b) (distance(a, b) <= TOLERANCE)

#define DEFAULT_COLOR vec4(0.0, 0.0, 0.0, 1.0)
#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE vec4(0.0, 0.0, 1.0, 1.0)
#define SAND vec4(0.76, 0.69, 0.5, 1.0)

vec2 offset[3] = {vec2(TEXEL_SIZE_X, 0), vec2(0, TEXEL_SIZE_Y), vec2(TEXEL_SIZE_X, TEXEL_SIZE_Y)};

// 0 - right; 1 - down; 2 - diagonal

int getTileValue(vec4 topLeft, vec4 topRight, vec4 bottomLeft, vec4 bottomRight)
{
    int tileValue = 0;

    if (equals(topLeft, SAND))
    {
        tileValue |= 1;
    }
    if (equals(topRight, SAND))
    {
        tileValue |= (1 << 1);
    }
    if (equals(bottomLeft, SAND))
    {
        tileValue |= (1 << 2);
    }
    if (equals(bottomRight, SAND))
    {
        tileValue |= (1 << 3);
    }

    return tileValue;
}

vec4 getTileColor(vec4 topLeft, vec4 topRight, vec4 bottomLeft, vec4 bottomRight)
{
    vec4 newColor = texture(outTexture, outTextureCoord);

    int tileValue = getTileValue(topLeft, topRight, bottomLeft, bottomRight);

    if (tileValue == 0)
    {
        return newColor;
    }

    switch (tileValue % 3)
    {
        case 0:
            newColor = GREEN;
            break;
        case 1:
            newColor = BLUE;
            break;
        case 2:
            newColor = RED;
            break;
    }

    return newColor;
}

void main()
{   
    vec4 topLeft = texture(outTexture, outTextureCoord); // current pixel
    vec4 topRight = texture(outTexture, outTextureCoord + offset[0]); // pixel to the right of the current one
    vec4 bottomLeft = texture(outTexture, outTextureCoord + offset[1]); // pixel below current one
    vec4 bottomRight = texture(outTexture, outTextureCoord + offset[2]); // pixel diagonal to current one
    
    vec4 newColor = getTileColor(topLeft, topRight, bottomLeft, bottomRight);

    if (mousePressed && mouseEquals(outTextureCoord, mouseCoord))
    {
        newColor = SAND;
    }

    FragColor = newColor;
}