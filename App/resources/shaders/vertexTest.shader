#version 410 core
layout (location = 0) in vec2 attrPosition;
layout (location = 1) in float attrValue;


out vec3 colour;

uniform float u_maxValue;
uniform float u_minValue;

void main()
{
    gl_Position = vec4(attrPosition, 0.0, 1.0);
    
    float relValue = attrValue - u_minValue;
    float factor = relValue / (u_maxValue - u_minValue);
    colour = vec3(0.0, 0.0, factor);
}