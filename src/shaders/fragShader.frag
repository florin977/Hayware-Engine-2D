#version 460 core

in vec3 outColor;

in vec2 outTextureCoord;

uniform int currentStep;

uniform sampler2D outTexture;
uniform ivec2 mouseCoord;
uniform bool mousePressed;

out vec4 FragColor;

#define TEXTURE_WIDTH 1000
#define TEXTURE_HEIGHT 800

#define TOLERANCE 0.05
#define MOUSE_PRESS_TOLERANCE 0.005
#define TEXEL_SIZE_X 1
#define TEXEL_SIZE_Y 1
#define mouseEquals(a, b) (distance(a, b) <= MOUSE_PRESS_TOLERANCE)
#define equals(a, b) (distance(a, b) <= TOLERANCE)

#define DEFAULT_COLOR vec4(0.0, 0.0, 0.0, 1.0)
#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE vec4(0.0, 0.0, 1.0, 1.0)
#define WHITE vec4(1.0)
#define WALL vec4(0.0, 0.0, 0.0, 0.0)
#define SAND vec4(0.76, 0.69, 0.5, 1.0)

ivec2 offset[3] = {ivec2(TEXEL_SIZE_X, 0), ivec2(0, TEXEL_SIZE_Y), ivec2(TEXEL_SIZE_X, TEXEL_SIZE_Y)};

// 0 - right; 1 - down; 2 - diagonal

bool inRange(vec2 currentCoord)
{
    return (all(greaterThanEqual(currentCoord, vec2(0.0, 0.0))) && 
            all(lessThanEqual(currentCoord, vec2(1.0, 1.0))));
}

ivec2 toIntegerPosition(vec2 floatPosition)
{
    return ivec2(floatPosition * vec2(TEXTURE_WIDTH, TEXTURE_HEIGHT));
}

vec2 toFloatPosition(ivec2 integerPosition)
{
    if (TEXTURE_WIDTH != 0 && TEXTURE_HEIGHT != 0)
    {
        return vec2(integerPosition) / ivec2(TEXTURE_WIDTH, TEXTURE_HEIGHT);
    }
    
    return vec2(0.0, 0.0);
}

int getTileValue(ivec2 topLeftPosition, ivec2 topRightPosition, ivec2 bottomLeftPosition, ivec2 bottomRightPosition)
{
    int tileValue = 0;

    vec4 topLeft = texture(outTexture, toFloatPosition(topLeftPosition));
    vec4 topRight = texture(outTexture, toFloatPosition(topRightPosition));
    vec4 bottomLeft = texture(outTexture, toFloatPosition(bottomLeftPosition));
    vec4 bottomRight = texture(outTexture, toFloatPosition(bottomRightPosition));

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

vec4 getTileColor(ivec2 topLeftPosition, ivec2 topRightPosition, ivec2 bottomLeftPosition, ivec2 bottomRightPosition)
{
    vec4 newColor = texture(outTexture, outTextureCoord);

    int tileValue = getTileValue(topLeftPosition, topRightPosition, bottomLeftPosition, bottomRightPosition);

    if (tileValue == 0)
    {
        return newColor;
    }

    switch (tileValue % 3)
    {
        case 0:
            newColor = SAND;
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

ivec2 getTexelPositions(out ivec2 topLeftPosition, out ivec2 topRightPosition, out ivec2 bottomLeftPosition, out ivec2 bottomRightPosition)
{
    switch (currentStep)
    {
        default:
            topLeftPosition = toIntegerPosition(outTextureCoord);
            topRightPosition = topLeftPosition + offset[0];
            bottomLeftPosition = topLeftPosition + offset[1];
            bottomRightPosition = topLeftPosition + offset[2];
            return topLeftPosition;
            break;

        /*case 0:
            topLeftPosition = toIntegerPosition(outTextureCoord);
            topRightPosition = topLeftPosition + offset[0];
            bottomLeftPosition = topLeftPosition + offset[1];
            bottomRightPosition = topLeftPosition + offset[2];
            return topLeftPosition;
            break;

        case 1:
            topRightPosition = toIntegerPosition(outTextureCoord);
            topLeftPosition = topRightPosition - offset[0];
            bottomLeftPosition = topLeftPosition + offset[1];
            bottomRightPosition = topLeftPosition + offset[2];
            return topRightPosition;
            break;
            */
    }
}

void main()
{   
    ivec2 topLeftPosition;
    ivec2 topRightPosition;
    ivec2 bottomLeftPosition;
    ivec2 bottomRightPosition;

    ivec2 currentPixelPosition = getTexelPositions(topLeftPosition, topRightPosition, bottomLeftPosition, bottomRightPosition);

    vec4 newColor = texture(outTexture, outTextureCoord);

    if (mousePressed && currentPixelPosition.x == mouseCoord.x && currentPixelPosition.y == mouseCoord.y)
    {
        newColor = SAND;
    }
    else
    {
        newColor = getTileColor(topLeftPosition, topRightPosition, bottomLeftPosition, bottomRightPosition);
    }

    FragColor = newColor;
}