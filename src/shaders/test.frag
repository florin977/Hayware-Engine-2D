#version 460 core

in vec3 outColor;

in vec2 outTextureCoord;

uniform int currentPass;

uniform sampler2D outTexture;
uniform ivec2 mouseCoord;
uniform bool mousePressed;

out vec4 FragColor;

#define TEXTURE_WIDTH 1000
#define TEXTURE_HEIGHT 800

#define TOLERANCE 0.01
#define MOUSE_PRESS_TOLERANCE 0.0001
#define TEXEL_SIZE_X (1.0 / float(TEXTURE_WIDTH))
#define TEXEL_SIZE_Y (1.0 / float(TEXTURE_HEIGHT))
#define NEIGHBORHOOD_SIZE 4
#define mouseEquals(a, b) (distance(a, b) <= MOUSE_PRESS_TOLERANCE)
#define equals(a, b) (distance(a, b) <= TOLERANCE)
#define colorEquals(A, B) (A.r == B.r && A.g == B.g && A.b == B.b && A.a == B.a)

#define DEFAULT_COLOR vec4(0.0, 0.0, 0.0, 1.0)
#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE vec4(0.0, 0.0, 1.0, 1.0)
#define WHITE vec4(1.0)
#define AIR vec4(0.0, 0.0, 0.0, 0.0)
#define WALL vec4(0.0, 0.0, 0.0, 1.0)
#define SAND vec4(0.76, 0.69, 0.5, 1.0)

int expectedPass = 0;
vec2 offset[4] = {vec2(0.0, 0.0), vec2(TEXEL_SIZE_X, 0.0), vec2(0.0, -TEXEL_SIZE_Y), vec2(TEXEL_SIZE_X, -TEXEL_SIZE_Y)};

// 0 - current; 1 - to the right; 2 - downward; 3 - diagonal

bool isOrigin(ivec2 iCoord)
{
    if ((iCoord.x & 1) == 0 && (iCoord.y & 1) == 1)
    {
        expectedPass = 0;
    }
    else if ((iCoord.x & 1) == 1 && (iCoord.y & 1) == 1)
    {
        expectedPass = 1;
    }
    else if ((iCoord.x & 1) == 0 && (iCoord.y & 1) == 0)
    {
        expectedPass = 2;
    }
    else 
    {
        expectedPass = 3;
    }

    return currentPass == expectedPass;
}

vec2 getOriginOffset(ivec2 iCoord)
{
    vec2 originOffset = vec2(0.0, 0.0);

    ivec2 intOffset[4] = {ivec2(0.0, 0.0), ivec2(1.0, 0.0), ivec2(0.0, -1.0), ivec2(1.0, -1.0)};

    if (isOrigin(iCoord))
    {
        return originOffset;
    }

    if (currentPass == 0 || currentPass == 3)
    {
        switch(abs(currentPass - expectedPass))
        {
            case 1:
            originOffset = vec2(-TEXEL_SIZE_X, 0.0);
            break;

            case 2:
            originOffset = vec2(0.0, +TEXEL_SIZE_Y);
            break;

            case 3:
            originOffset = vec2(-TEXEL_SIZE_X, TEXEL_SIZE_Y);
            break;
        }
    }
    else if (currentPass == 1)
    {
        switch(currentPass - expectedPass)
        {
            case 1:
            originOffset = vec2(-TEXEL_SIZE_X, 0.0);
            break;

            case -2:
            originOffset = vec2(0.0, +TEXEL_SIZE_Y);
            break;

            case -1:
            originOffset = vec2(-TEXEL_SIZE_X, TEXEL_SIZE_Y);
            break;
        }
    }
    else 
    {
        switch(currentPass - expectedPass)
        {
            case -1:
            originOffset = vec2(-TEXEL_SIZE_X, 0.0);
            break;

            case 2:
            originOffset = vec2(0.0, +TEXEL_SIZE_Y);
            break;

            case 1:
            originOffset = vec2(-TEXEL_SIZE_X, TEXEL_SIZE_Y);
            break;
        }
    }

    return originOffset;
}

vec4 testOrigin(ivec2 iCoord)
{
    if (isOrigin(iCoord))
    {
        return SAND;
    }
    
    return WALL;
}

vec4 testDistanceToOrigin(ivec2 iCoord)
{
    vec2 originOffset = getOriginOffset(iCoord);

    if (originOffset == vec2(0.0, 0.0))
    {
        return SAND;
    }
    else if (originOffset == vec2(-TEXEL_SIZE_X, 0.0))
    {
        return RED;
    }
    else if (originOffset == vec2(0.0, TEXEL_SIZE_Y))
    {
        return GREEN;
    }
    else 
    {
        return BLUE;
    }
}

void main()
{   
    vec4 newColor = texture(outTexture, outTextureCoord);
    ivec2 iCoord = ivec2(outTextureCoord * ivec2(TEXTURE_WIDTH, TEXTURE_HEIGHT));

    //newColor = testOrigin(iCoord);
    newColor = testDistanceToOrigin(iCoord);

    FragColor = newColor;
}