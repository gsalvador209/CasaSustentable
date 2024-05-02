#version 330 core

#extension GL_NV_shadow_samplers_cube : enable

out vec4 FragColor;

in vec3 TexCoords;

uniform samplerCube skybox;

uniform vec4 LightColor;


void main()
{    
    vec4 textureColor = texture(skybox, TexCoords);
    FragColor = textureColor * LightColor * 5.0;
}