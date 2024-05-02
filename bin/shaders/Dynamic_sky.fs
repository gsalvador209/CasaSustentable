#version 330 core

#extension GL_NV_shadow_samplers_cube : enable

out vec4 FragColor;

in vec3 TexCoords;

in vec3 EyeDirection_cameraspace;
in vec3 LightDirection_cameraspace;
in vec3 Normal_cameraspace;



uniform samplerCube skybox;

uniform vec4 LightColor;
uniform vec4 LightPower;
uniform int  alphaIndex;

uniform vec4 MaterialSpecularColor;


void main()
{    
    vec3 n = normalize( Normal_cameraspace );
    vec3 l = normalize( LightDirection_cameraspace );

    vec3 E = normalize(EyeDirection_cameraspace);
    vec3 R = reflect(-l,n);
    float cosAlpha = clamp(dot(E,R), 0, 1 );
    vec4 K_s = 10000 * LightColor * pow(cosAlpha,0.1);



    vec4 textureColor = texture(skybox, TexCoords);
    FragColor = textureColor * LightColor * 5.0 + K_s*LightPower;
}