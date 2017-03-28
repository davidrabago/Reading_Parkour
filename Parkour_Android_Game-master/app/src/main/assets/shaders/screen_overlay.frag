precision mediump float;

//X component: black opacity
//Y component: white opacity
//Z component: subtle fade overlay opacity
uniform vec4 info;

uniform float view_width;
uniform float view_height;

void main()
{
	//Fade to black on the edges:
	vec2 uv = (2.0 * vec2(gl_FragCoord.x / view_width, gl_FragCoord.y/view_height)) - vec2(1.0,1.0);
	vec4 fade_color = vec4(0.0,0.0,0.0,0.0);
	//fade_color.a = (pow(uv.x,4.0) + pow(uv.y,4.0));
	fade_color.a = info.z * 0.01 + 0.3 * (pow(uv.x,4.0) + pow(uv.y,4.0));

	gl_FragColor = (((fade_color * (1.0 - info.x)) + (info.x * vec4(0.0,0.0,0.0,1.0))) * (1.0 - info.y)) + info.y * vec4(1.0,1.0,1.0,1.0);
}