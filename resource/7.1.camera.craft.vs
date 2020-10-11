#version 300 es

#ifdef GL_ES
precision highp float;
#endif

in vec3 position;
in vec3 normal;
in vec2 uv;

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
	gl_Position = projection * view * model * vec4(position, 1.0f);
	TexCoord = vec2(uv.x, uv.y);
}