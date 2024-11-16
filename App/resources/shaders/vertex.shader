#version 410 core
layout (location = 0) in vec2 attrPosition;
layout (location = 1) in vec3 attrColour;

out vec3 colour;

void main()
{
    gl_Position = vec4(attrPosition, 0.0, 1.0);
    colour = attrColour;
}