attribute vec4 vert_pos;

uniform vec4 info;

uniform float view_width;
uniform float view_height;

void main()
{
	vec4 pos;
	pos = vert_pos;
	pos.w = 1.0;
	gl_Position = pos;
}