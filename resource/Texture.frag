#version 300 es
#ifdef GL_ES
precision highp float;
#endif

in vec3 position;
in vec3 normal;
in vec2 uv;


uniform mat4 camera;
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec2 TexCoord;

void main()
{
    //gl_Position = projection * view * model * vec4(aPos, 1.0f);
    gl_Position = vec4(aPos, 1.0f);
    TexCoord = uv;
}


