attribute vec4 vert_pos;
attribute vec2 vert_uv;

varying vec2 uv;

void main()
{
	vec4 pos;
	pos = vert_pos;
	pos.w = 1.0;
	gl_Position = pos;

	uv = vert_uv;
}