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
#define MOUSE_PRESS_TOLERANCE 0.005
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

ivec2 iOffset[4] = {ivec2(0.0, 0.0), ivec2(1.0, 0.0), ivec2(0.0, -1.0), ivec2(1.0, -1.0)}; // Why not -1 ???
vec2 flOffset[4] = {vec2(0.0, 0.0), vec2(TEXEL_SIZE_X, 0.0), vec2(0.0, -TEXEL_SIZE_Y), vec2(TEXEL_SIZE_X, -TEXEL_SIZE_Y)};

// 0 - current; 1 - right; 2 - down; 3 - diagonal

void getCurrentNeighborhood(ivec2 origin, vec2 flOrigin, out ivec2 iCurrentNeighborhood[NEIGHBORHOOD_SIZE], out vec2 flCurrentNeighborhood[NEIGHBORHOOD_SIZE])
{
    for (int i = 0; i < NEIGHBORHOOD_SIZE; i++)
    {
        iCurrentNeighborhood[i] = ivec2(origin.x + iOffset[i].x, origin.y + iOffset[i].y);
        flCurrentNeighborhood[i] = vec2(flOrigin.x + flOffset[i].x, flOrigin.y + flOffset[i].y);
    }
}

bool inRange(ivec2 position)
{
    return (0 <= position.x && position.x <= TEXTURE_WIDTH && 0 <= position.y && position.y <= TEXTURE_HEIGHT);
}

int getNeighborhoodValue(ivec2 iCurrentNeighborhood[NEIGHBORHOOD_SIZE], vec2 flCurrentNeighborhood[NEIGHBORHOOD_SIZE])
{
    int value = 0;

    for (int i = 0; i < NEIGHBORHOOD_SIZE; i++)
    {
        vec4 elementColor = texture(outTexture, flCurrentNeighborhood[i]);

        if (equals(elementColor, SAND))
        {
            value = value | (1 << i);
        }
    }

    return value;
}

ivec3 getOriginOffset(ivec2 iCoord, out vec2 flOffset)
{
    ivec3 offset = ivec3(0.0, 0.0, 0.0); // 1st and 2nd: offset 3rd : final state index

    int expectedPass = 0;

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

    // Handle possible cases

    if (currentPass == expectedPass)
    {
        offset = ivec3(0.0, 0.0, 0.0);
        flOffset = vec2(0.0, 0.0);
    }
    if (currentPass == 0 && expectedPass == 1)
    {
        offset = ivec3(0.0, 1.0, 2.0);
        flOffset = vec2(0.0, TEXEL_SIZE_Y);
    }
    if (currentPass == 0 && expectedPass == 2)
    {
        offset = ivec3(-1.0, 0.0, 1.0);
        flOffset = vec2(-TEXEL_SIZE_X, 0.0);
    }
    if (currentPass == 0 && expectedPass == 3)
    {
        offset = ivec3(-1.0, 1.0, 3.0);
        flOffset = vec2(-TEXEL_SIZE_X, TEXEL_SIZE_Y);
    }

    if (currentPass == 1 && expectedPass == 0)
    {
        offset = ivec3(0.0, 1.0, 2.0);
        flOffset = vec2(0.0, TEXEL_SIZE_Y);
    }
    if (currentPass == 1 && expectedPass == 2)
    {
        offset = ivec3(-1.0, 1.0, 3.0);
        flOffset = vec2(-TEXEL_SIZE_X, TEXEL_SIZE_Y);
    }
    if (currentPass == 1 && expectedPass == 3)
    {
        offset = ivec3(-1.0, 0.0, 1.0);
        flOffset = vec2(-TEXEL_SIZE_X, 0.0);
    }

    if (currentPass == 2 && expectedPass == 0)
    {
        offset = ivec3(-1.0, 0.0, 1.0);
        flOffset = vec2(-TEXEL_SIZE_X, 0.0);
    }
    if (currentPass == 2 && expectedPass == 1)
    {
        offset = ivec3(-1.0, 1.0, 3.0);
        flOffset = vec2(-TEXEL_SIZE_X, TEXEL_SIZE_Y);
    }
    if (currentPass == 2 && expectedPass == 3)
    {
        offset = ivec3(0.0, 1.0, 2.0);
        flOffset = vec2(0.0, TEXEL_SIZE_Y);
    }

    if (currentPass == 3 && expectedPass == 0)
    {
        offset = ivec3(-1.0, 1.0, 3.0);
        flOffset = vec2(-TEXEL_SIZE_X, TEXEL_SIZE_Y);
    }
    if (currentPass == 3 && expectedPass == 1)
    {
        offset = ivec3(-1.0, 0.0, 1.0);
        flOffset = vec2(-TEXEL_SIZE_X, 0.0);
    }
    if (currentPass == 3 && expectedPass == 2)
    {
        offset = ivec3(0.0, 1.0, 2.0);
        flOffset = vec2(0.0, TEXEL_SIZE_Y);
    }

    return offset;
}

vec4 updatePixelState(ivec2 iCoord)
{
    vec2 flOriginOffset = vec2(0.0, 0.0);
    ivec3 originOffset = getOriginOffset(iCoord, flOriginOffset);

    ivec2 iCurrentNeighborhood[NEIGHBORHOOD_SIZE] = {ivec2(0.0, 0.0), ivec2(0.0, 0.0), ivec2(0.0, 0.0), ivec2(0.0, 0.0)};
    vec2 flCurrentNeighborhood[NEIGHBORHOOD_SIZE] = {vec2(0.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 0.0)};

    getCurrentNeighborhood(iCoord + originOffset.xy, outTextureCoord + flOriginOffset.xy, iCurrentNeighborhood, flCurrentNeighborhood);

    int neighborhoodValue = getNeighborhoodValue(iCurrentNeighborhood, flCurrentNeighborhood);

    vec4 finalState[NEIGHBORHOOD_SIZE];

    for (int i = 0; i < NEIGHBORHOOD_SIZE; i++)
    {
        vec4 currentColor = texture(outTexture, flCurrentNeighborhood[i]);

        /*if (outTextureCoord.x == flCurrentNeighborhood[i].x && outTextureCoord.y == flCurrentNeighborhood[i].y)
        {
            return GREEN;
        }*/

        finalState[i] = currentColor;
    }
    
    switch (neighborhoodValue)
    {
        case 1:
            finalState[0] = AIR;
            finalState[1] = AIR;
            finalState[2] = SAND;
            finalState[3] = AIR;
            break;
        case 2:
            finalState[0] = AIR;
            finalState[1] = AIR;
            finalState[2] = AIR;
            finalState[3] = SAND;
            break;
        case 3:
        case 5:
        case 6:
        case 9:
        case 10:
            finalState[0] = AIR;
            finalState[1] = AIR;
            finalState[2] = SAND;
            finalState[3] = SAND;
            break;
        case 7:
            finalState[0] = SAND;
            finalState[1] = AIR;
            finalState[2] = SAND;
            finalState[3] = SAND;
            break;
        case 11:
            finalState[0] = AIR;
            finalState[1] = SAND;
            finalState[2] = SAND;
            finalState[3] = SAND;
            break;
        default:
            break;
    }
    if (neighborhoodValue != 0.0)
    {
        finalState[0] = RED;
        finalState[1] = RED;
        finalState[2] = RED;
        finalState[3] = RED;
    }

    return finalState[originOffset.z];
}

void main()
{   
    vec4 newColor = texture(outTexture, outTextureCoord);

    ivec2 iCoord = ivec2(outTextureCoord.x * TEXTURE_WIDTH, outTextureCoord.y * TEXTURE_HEIGHT);

    newColor = updatePixelState(iCoord);

    if (mousePressed && iCoord.x == mouseCoord.x && iCoord.y == mouseCoord.y)
    {
        newColor = SAND;
    }

    if ((iCoord.x % 10 == 0 && iCoord.y % 10 == 0) )//|| (iCoord.x % 10 == 0 && iCoord.y % 10 == 1) || (iCoord.x % 10 == 1 && iCoord.y % 10 == 0) || (iCoord.x % 10 == 1 && iCoord.y % 10 == 1))
    {
        newColor = SAND;
    }

    FragColor = newColor;
}