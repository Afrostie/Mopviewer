#version 330 core
in vec3 ourColor;
in vec2 TexCoord;

out vec4 color;

uniform sampler2D myTexture1;
uniform sampler2D myTexture2;
uniform float mixValue;

void main()
{
    color = mix(texture(myTexture1, TexCoord), texture(myTexture2, vec2(1.0 - TexCoord.x, TexCoord.y)), mixValue);
}