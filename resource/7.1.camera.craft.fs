#version 300 es

#ifdef GL_ES
precision highp float;
#endif

out vec4 FragColor;

in vec2 TexCoord;

// texture samplers
uniform sampler2D texture1;

void main()
{
	vec3 color = vec3(texture(texture1, TexCoord));
    if (color == vec3(1.0, 0.0, 1.0)) {
        discard;
    }
    FragColor = vec4(color, 1.0);
}