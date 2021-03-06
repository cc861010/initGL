#version 300 es
#ifdef GL_ES
precision highp float;
#endif
out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D sampler;


void main()
{
    FragColor = texture(sampler, TexCoord);
}