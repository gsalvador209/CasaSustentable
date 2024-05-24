#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 ex_N; 

uniform sampler2D texture_diffuse1;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform int frame;

void main()
{   
    vec2 coordsT = TexCoords;
    coordsT.y += 0.05882 * frame;
    vec4 texel = texture(texture_diffuse1, coordsT);
    if(texel.a < 0.5) {
        discard;
    }
    FragColor = texel;

}