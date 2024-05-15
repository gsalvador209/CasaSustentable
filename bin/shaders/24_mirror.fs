#version 330 core
out vec4 FragColor;


uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float show;

void main()
{    
    vec4 FragColor = vec4(1.0f,1.0f,1.0f,1.0f);
}