uniform mat4 mvp;

uniform mat3 m_IT;

attribute vec4 vert_pos;


void main()
{
	vec4 pos;
	pos = vert_pos;
	pos.w = 1.0;
	gl_Position = mvp * pos;
}