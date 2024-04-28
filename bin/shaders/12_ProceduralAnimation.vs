#version 330 core
layout (location = 0) in vec3  aPos;
layout (location = 1) in vec3  aNormal;
layout (location = 2) in vec2  aTexCoords;
layout (location = 3) in vec3  tangent;
layout (location = 4) in vec3  bitangent;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

uniform float time;
uniform float height;
uniform float k;

void main()
{
    
    vec4 PosL = vec4(aPos, 1.0f);
    PosL.x += time * cos(time);
    PosL.y += time * sin(time);
    PosL.z += height + k*time;

    gl_Position = projection * view * model * PosL;

    TexCoords = aTexCoords;  
}