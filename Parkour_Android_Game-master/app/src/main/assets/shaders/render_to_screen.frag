precision mediump float;

varying vec2 uv;

uniform sampler2D tex_diff;

//uniform float view_width;
//uniform float view_height;

void main()
{
//	vec2 uv = vec2(gl_FragCoord.x / view_width, gl_FragCoord.y/view_height);
	gl_FragColor = vec4(texture2D(tex_diff,uv).rgb,1.0);
}