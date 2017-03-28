precision mediump float;

varying vec2 v_uv_1;
varying vec2 v_uv_2;

varying vec3 cam_dir_tanspace;
varying vec3 dirlight_dir_tanspace;
varying vec3 cam_to_vert_tanspace;
varying mat3 tan_to_world;

uniform sampler2D tex_nor;
uniform sampler2D tex_diff;
uniform sampler2D tex_lm;
uniform sampler2D tex_misc;
uniform samplerCube cube_map;

uniform float time;


void main()
{
	//gl_FragColor = color*0.01 + v_color*0.01 + vec4( v_uv_1.x , v_uv_1.y , v_uv_2.x , v_uv_2.y );
	//Getting fragment normal
	//Modulating the normal map strength (lerping between unmodified normal and normal specified by normal map by strength amount)
	const float normal_map_strength = 0.3;
	vec3 normal_map_dir = (texture2D(tex_nor,v_uv_1).rgb * 2.0 - 1.0);
	vec3 normal_dir = normalize(mix(vec3(0,0,1),normal_map_dir,normal_map_strength));

	vec3 ref_dir = tan_to_world * reflect(cam_to_vert_tanspace,normal_dir);
	vec3 ref_color = textureCube(cube_map,ref_dir).xyz;

	//Getting misc texture map data
	//R: reflectivity
	//G: not articulated
	//B: not articulated
	vec3 misc_data = texture2D(tex_misc,v_uv_1).rgb;

	//Light Calculation
	//(TODO: Should I add a small diffuse contribution for highlighting the normal map?) (todo once I approach final shader results)
	//float diffuse = clamp(dot(normal_map_dir,dirlight_dir_tanspace),0.0,1.0);

	//Specular shading
	const float shininess = 5.0;//ranged 1-20
	float specular = pow(clamp(dot(reflect(-dirlight_dir_tanspace,normal_dir),cam_dir_tanspace),0.0,1.0),shininess);

	//Rim light shading
	//const float rim_power = 3.0;//ranged 0.1-10
	//float rim = 1.0 - clamp(dot(cam_dir_tanspace,normal_dir),0.0,1.0);
	//rim = pow(rim,rim_power);

	const float ambient_light = 0.1;

	//Lightmaps are currently monochromatic
	float lightmap_brightness = texture2D(tex_lm,v_uv_2).r;
	//float light_power = ambient_light + diffuse + specular + rim;
	//float light_power = ambient_light + 0.7*specular + rim + lightmap_brightness;
	float light_power = ambient_light + 0.7*specular + lightmap_brightness;

	vec3 color = mix(texture2D(tex_diff,v_uv_1).rgb,ref_color,misc_data.r);

	gl_FragColor = vec4(color*light_power, 1.0);
}