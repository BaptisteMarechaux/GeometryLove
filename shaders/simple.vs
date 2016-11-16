uniform mat4 MVP;
attribute vec2 position;

void main()
{
    gl_Position = MVP * vec4(position, 0.0, 1.0);
}