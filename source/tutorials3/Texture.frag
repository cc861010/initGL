#version 300 es
#ifdef GL_ES
precision highp float;
#endif

out vec4 FragColor;

in vec3 ourColor;
in vec2 TexCoord;

// texture sampler
uniform sampler2D texture1;

void main()
{
    FragColor = texture2D(texture1, TexCoord);
}