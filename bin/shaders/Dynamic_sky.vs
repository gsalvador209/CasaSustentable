#version 330 core

#extension GL_NV_shadow_samplers_cube : enable

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3  aNormal;

out vec3 TexCoords;


out vec3 EyeDirection_cameraspace;
out vec3 LightDirection_cameraspace;
out vec3 Normal_cameraspace;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;

uniform vec3 lightPosition;
uniform vec3 lightDirection;
uniform vec3 eye;

void main()
{
    TexCoords = aPos;
    gl_Position = projection * view * vec4(aPos, 1.0f);

    vec3 vertexPosition_cameraspace = ( view * model * vec4(aPos,1)).xyz;
    EyeDirection_cameraspace = vec3(0,0,0) - vertexPosition_cameraspace;
    vec3 LightPosition_cameraspace = ( view * vec4(lightPosition,1)).xyz;
    LightDirection_cameraspace = LightPosition_cameraspace + EyeDirection_cameraspace;
    Normal_cameraspace = ( view * vec4(0,1,0,0)).xyz;
}  