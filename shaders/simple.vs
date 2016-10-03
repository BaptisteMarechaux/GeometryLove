uniform mat4 MVP;
attribute vec3 color_in;
attribute vec2 position;
varying vec3 color_out;

void main()
{
    gl_Position = MVP * vec4(position, 0.0, 1.0);
    color_out = color_in;
}