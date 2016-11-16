uniform mat4 MVP;
uniform vec3 color_in;
attribute vec2 position;
varying vec3 color_out;

//out vec3 fragmentColor;

void main()
{
    gl_Position = MVP * vec4(position, 0.0, 1.0);
    color_out = color_in;
	//fragmentColor = color_in;
}