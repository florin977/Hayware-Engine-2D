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
#define AIR vec4(0.0, 0.0, 0.0, 0.0)
#define WALL vec4(0.0, 0.0, 0.0, 1.0)
#define SAND vec4(0.76, 0.69, 0.5, 1.0)

int expectedPass = 0;
int stateIndex = 0;

// 0 - current; 1 - to the right; 2 - downward; 3 - diagonal
vec2 gridOffsets[GRID_SIZE] = {vec2(0.0, 0.0), vec2(TEXEL_SIZE_X, 0.0), vec2(0.0, -TEXEL_SIZE_Y), vec2(TEXEL_SIZE_X, -TEXEL_SIZE_Y)};

vec2 gridCells[GRID_SIZE] = {vec2(0.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 0.0), vec2(0.0, 0.0)};
vec4 gridColors[GRID_SIZE] = {GREEN, GREEN, GREEN, GREEN};

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

void getGridColors(vec2 origin)
{
    for (int i = 0; i < GRID_SIZE; i++)
    {
        gridCells[i] = origin + gridOffsets[i];

        gridColors[i] = texture(outTexture, gridCells[i]);
        
        if (!inRange(gridCells[i]))
        {
            gridColors[i] = WALL;
        }
    }
}

int getGridValue(vec2 origin)
{
    int gridValue = 0;

    getGridColors(origin);

    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (equals(gridColors[i], SAND))
        {
            gridValue = gridValue | (1 << i);
        }
        else if (equals(gridColors[i], WALL))
        {
            gridValue = 15;
            break;
        }
    }

    return gridValue;
}

vec4 updatePixelColor(ivec2 iCoord)
{
    vec2 origin = outTextureCoord + getOriginOffset(iCoord);
    getGridColors(origin);

    int gridValue = getGridValue(origin);

    vec4 updatedGridColors[4] = gridColors;

    switch (gridValue)
    {
        case 1:
            updatedGridColors[0] = AIR;
            updatedGridColors[1] = AIR;
            updatedGridColors[2] = SAND;
            updatedGridColors[3] = AIR;
            break;
        case 2:
            updatedGridColors[0] = AIR;
            updatedGridColors[1] = AIR;
            updatedGridColors[2] = AIR;
            updatedGridColors[3] = SAND;
            break;
        case 3:
        case 5:
        case 6:
        case 9:
        case 10:
            updatedGridColors[0] = AIR;
            updatedGridColors[1] = AIR;
            updatedGridColors[2] = SAND;
            updatedGridColors[3] = SAND;
            break;
        case 7:
            updatedGridColors[0] = SAND;
            updatedGridColors[1] = AIR;
            updatedGridColors[2] = SAND;
            updatedGridColors[3] = SAND;
            break;
        case 11:
            updatedGridColors[0] = AIR;
            updatedGridColors[1] = SAND;
            updatedGridColors[2] = SAND;
            updatedGridColors[3] = SAND;
            break;
    }

    return updatedGridColors[stateIndex];
}

vec4 checkSandSpawn(ivec2 iCoord, vec4 currentColor)
{
    if (mousePressed && iCoord == mouseCoord)
    {
        return SAND;
    }

    return currentColor;
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

vec4 testStateIndex(ivec2 iCoord)
{
    vec2 originOffset = getOriginOffset(iCoord);

    switch(stateIndex)
    {
        case 0:
        return SAND;
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

vec4 visualizeGrid(ivec2 iCoord)
{
    vec2 origin = outTextureCoord + getOriginOffset(iCoord);
    getGridColors(origin);

    int gridValue = getGridValue(origin);

    vec4 updatedGridColors[4] = gridColors;

    for (int i = 0; i < GRID_SIZE; i++)
    {
        if (equals(updatedGridColors[i], SAND))
        {
            return WHITE;
        }
    }

    return AIR;
}

void main()
{   
    vec4 newColor = texture(outTexture, outTextureCoord);
    ivec2 iCoord = ivec2(outTextureCoord * ivec2(TEXTURE_WIDTH, TEXTURE_HEIGHT));


    // TESTS
    //newColor = testOrigin(iCoord);
    //newColor = testDistanceToOrigin(iCoord);
    /*
    if (!mousePressed)
    {
        newColor = testGridValue(iCoord, newColor);
    }
    */
    //newColor = visualizeGrid(iCoord);
    //-------

    newColor = updatePixelColor(iCoord);
    newColor = checkSandSpawn(iCoord, newColor);

    FragColor = newColor;
}