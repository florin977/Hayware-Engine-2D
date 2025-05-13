#version 460 core

in vec3 outColor;

in vec2 outTextureCoord;

uniform int currentPass;

uniform sampler2D outTexture;
uniform usampler2D outTextureID;
uniform sampler2D outTextureVelocity;

uniform ivec2 mouseCoord;
uniform bool mousePressed;

layout(location = 0) out vec4 FragColor;
layout(location = 1) out uint FragID;
layout(location = 2) out vec2 FragVelocity;

#define TEXTURE_WIDTH 1000
#define TEXTURE_HEIGHT 800

#define TOLERANCE 0.01
#define GRID_TOLERANCE 0.0000001
#define TEXEL_SIZE_X (1.0 / float(TEXTURE_WIDTH))
#define TEXEL_SIZE_Y (1.0 / float(TEXTURE_HEIGHT))
#define GRID_SIZE 4
#define gridEquals(a, b) (distance(a, b) <= GRID_TOLERANCE)
#define equals(a, b) (distance(a, b) <= TOLERANCE)
#define colorEquals(A, B) (A.r == B.r && A.g == B.g && A.b == B.b && A.a == B.a)

#define DEFAULT_COLOR vec4(0.0, 0.0, 0.0, 1.0)
#define RED vec4(1.0, 0.0, 0.0, 1.0)
#define GREEN vec4(0.0, 1.0, 0.0, 1.0)
#define BLUE vec4(0.0, 0.0, 1.0, 1.0)
#define WHITE vec4(1.0)
#define AIR_COLOR vec4(0.0, 0.0, 0.0, 0.0)
#define WALL_COLOR vec4(0.0, 0.0, 0.0, 1.0)
#define SAND_COLOR vec4(0.76, 0.69, 0.5, 1.0)

#define WALL 100
#define AIR 0
#define SAND 1

int expectedPass = 0;
int stateIndex = 0;

// 0 - current; 1 - to the right; 2 - downward; 3 - diagonal
vec2 gridOffsets[GRID_SIZE] = {vec2(0.0, 0.0), vec2(TEXEL_SIZE_X, 0.0), vec2(0.0, -TEXEL_SIZE_Y), vec2(TEXEL_SIZE_X, -TEXEL_SIZE_Y)};

vec2 gridCells[GRID_SIZE] = {vec2(0.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 0.0)};
uint gridIDs[GRID_SIZE] = {0, 0, 0, 0};

bool inRange(vec2 position)
{
    return (0.0 <= position.x && position.x <= 1.0 && 0.0 <= position.y && position.y <= 1.0);
}

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

    if (isOrigin(iCoord))
    {
        stateIndex = 0;
        return originOffset;
    }

    if (currentPass == 0 || currentPass == 3)
    {
        switch(abs(currentPass - expectedPass))
        {
            case 1:
            stateIndex = 1;
            originOffset = vec2(-TEXEL_SIZE_X, 0.0);
            break;

            case 2:
            stateIndex = 2;
            originOffset = vec2(0.0, +TEXEL_SIZE_Y);
            break;

            case 3:
            stateIndex = 3;
            originOffset = vec2(-TEXEL_SIZE_X, TEXEL_SIZE_Y);
            break;
        }
    }
    else if (currentPass == 1)
    {
        switch(currentPass - expectedPass)
        {
            case 1:
            stateIndex = 1;
            originOffset = vec2(-TEXEL_SIZE_X, 0.0);
            break;

            case -2:
            stateIndex = 2;
            originOffset = vec2(0.0, +TEXEL_SIZE_Y);
            break;

            case -1:
            stateIndex = 3;
            originOffset = vec2(-TEXEL_SIZE_X, TEXEL_SIZE_Y);
            break;
        }
    }
    else if (currentPass == 2)
    {
        switch(currentPass - expectedPass)
        {
            case -1:
            stateIndex = 1;
            originOffset = vec2(-TEXEL_SIZE_X, 0.0);
            break;

            case 2:
            stateIndex = 2;
            originOffset = vec2(0.0, +TEXEL_SIZE_Y);
            break;

            case 1:
            stateIndex = 3;
            originOffset = vec2(-TEXEL_SIZE_X, TEXEL_SIZE_Y);
            break;
        }
    }

    return originOffset;
}

void getGridIDs(vec2 origin)
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        gridCells[i] = origin + gridOffsets[i];

        gridIDs[i] = texture(outTextureID, gridCells[i]).r;
        
        if (!inRange(gridCells[i]))
        {
            gridIDs[i] = WALL;
        }
    }
}

int getGridValue(vec2 origin)
{
    int gridValue = 0;

    getGridIDs(origin);

    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (gridIDs[i] == SAND)
        {
            gridValue = gridValue | (1 << i);
        }
        else if (gridIDs[i] == WALL)
        {
            gridValue = 15;
            break;
        }
    }

    return gridValue;
}

uint updatePixelID(ivec2 iCoord)
{
    vec2 origin = outTextureCoord + getOriginOffset(iCoord);

    int gridValue = getGridValue(origin);

    uint updatedGridIDs[4] = gridIDs;

    switch (gridValue)
    {
        case 1:
            updatedGridIDs[0] = AIR;
            updatedGridIDs[1] = AIR;
            updatedGridIDs[2] = SAND;
            updatedGridIDs[3] = AIR;
            break;
        case 2:
            updatedGridIDs[0] = AIR;
            updatedGridIDs[1] = AIR;
            updatedGridIDs[2] = AIR;
            updatedGridIDs[3] = SAND;
            break;
        case 3:
        case 5:
        case 6:
        case 9:
        case 10:
            updatedGridIDs[0] = AIR;
            updatedGridIDs[1] = AIR;
            updatedGridIDs[2] = SAND;
            updatedGridIDs[3] = SAND;
            break;
        case 7:
            updatedGridIDs[0] = SAND;
            updatedGridIDs[1] = AIR;
            updatedGridIDs[2] = SAND;
            updatedGridIDs[3] = SAND;
            break;
        case 11:
            updatedGridIDs[0] = AIR;
            updatedGridIDs[1] = SAND;
            updatedGridIDs[2] = SAND;
            updatedGridIDs[3] = SAND;
            break;
    }

    return updatedGridIDs[stateIndex];
}

uint checkSandSpawn(ivec2 iCoord, uint currentID)
{
    if (mousePressed && iCoord == mouseCoord)
    {
        return SAND;
    }

    return currentID;
}

void main()
{   
    vec4 newColor = texture(outTexture, outTextureCoord);
    uint newID = texture(outTextureID, outTextureCoord).r;
    vec2 newVelocity = texture(outTextureVelocity, outTextureCoord).xy;

    ivec2 iCoord = ivec2(outTextureCoord * ivec2(TEXTURE_WIDTH, TEXTURE_HEIGHT));

    newID = updatePixelID(iCoord);
    newID = checkSandSpawn(iCoord, newID);

    FragID = newID;

    FragVelocity = vec2(0.0, 0.0); // TODO: Use actual velocity of the sand

    if (newID == SAND)
    {
        FragColor = SAND_COLOR;
    }
    else if (newID == AIR)
    {
        FragColor = WALL_COLOR;
    }
    else 
    {
        FragColor = WHITE;
    }
}



// TESTS

vec4 testOrigin(ivec2 iCoord)
{
    if (isOrigin(iCoord))
    {
        return SAND_COLOR;
    }
    
    return WALL_COLOR;
}

vec4 testDistanceToOrigin(ivec2 iCoord)
{
    vec2 originOffset = getOriginOffset(iCoord);
    
    if (originOffset == vec2(0.0, 0.0))
    {
        return SAND_COLOR;
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

vec4 testStateIndex(ivec2 iCoord)
{
    vec2 originOffset = getOriginOffset(iCoord);

    switch(stateIndex)
    {
        case 0:
        return SAND_COLOR;
        break;

        case 1:
        return RED;
        break;

        case 2:
        return GREEN;
        break;

        case 3:
        return BLUE;
        break;
    }
}

vec4 testGridValue(ivec2 iCoord, vec4 currentColor)
{
    vec2 originOffset = getOriginOffset(iCoord);

    int gridValue = getGridValue(outTextureCoord + originOffset);

    switch(gridValue % 4)
    {
        case 0:
        return currentColor;

        case 1:
        return RED;

        case 2:
        return GREEN;

        case 3:
        return BLUE;
    }

    return currentColor;
}