#version 300 es

in vec2 position2d;
out vec4 color;
const vec4 white = vec4(1.0);

void main(){
    gl_Position = vec4(position2d, 0.0, 1.0);
    color = white/gl_Position;
}