#version 460 core
layout (location = 0) in vec3 aPos;

uniform float xAngle;
uniform float zAngle;

void main()
{
    mat3 xRotationMatrix = mat3(1.0,     0.0,        0.0,
                               0.0, cos(xAngle), -sin(xAngle),
                               0.0, sin(xAngle), cos(xAngle));

    mat3 zRotationMatrix = mat3(cos(zAngle), 0.0, sin(zAngle),
                                0.0,         1.0,     0.0,
                                -sin(zAngle), 0.0, cos(zAngle));

    vec3 rotatedPosition = xRotationMatrix * zRotationMatrix * aPos;     

    gl_Position = vec4(rotatedPosition, 1.0);
}

// Yellow: vec4(0.76, 0.73, 0.0, 1.0)
// Dark-Green: vec4(0.0f, 0.25f, 0.23f, 1.0f)