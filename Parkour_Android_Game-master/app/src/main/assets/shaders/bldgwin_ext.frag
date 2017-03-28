precision mediump float;

varying vec2 v_uv_1;

varying vec3 cam_dir_tanspace;
varying vec3 dirlight_dir_tanspace;
varying vec3 cam_to_vert_tanspace;
varying mat3 tan_to_world;

varying vec4 vert_world;
varying vec3 cam_to_vert_nrmlized;

uniform sampler2D tex_nor;
uniform sampler2D tex_diff;
uniform sampler2D tex_misc;
uniform samplerCube cube_map;

void main()
{
	//Getting fragment normal
	//Modulating the normal map strength (lerping between unmodified normal and normal specified by normal map by strength amount)
	const float normal_map_strength = 0.3;
	vec3 normal_map_dir = (texture2D(tex_nor,v_uv_1).rgb * 2.0 - 1.0);
	vec3 normal_dir = normalize(mix(vec3(0,0,1),normal_map_dir,normal_map_strength));

	vec3 ref_dir = tan_to_world * reflect(cam_to_vert_tanspace,normal_dir);
	vec3 ref_color = textureCube(cube_map,ref_dir).xyz;

	//Light Calculation
	//float diffuse = clamp(dot(normal_dir,dirlight_dir_tanspace),0.0,1.0);

	//Specular shading
	const float shininess = 5.0;//ranged 1-20
	float specular = pow(clamp(dot(reflect(-dirlight_dir_tanspace,normal_dir),cam_dir_tanspace),0.0,1.0),shininess);

	//Rim light shading
	//const float rim_power = 3.0;//ranged 0.1-10
	//float rim = 1.0 - clamp(dot(cam_dir_tanspace,normal_dir),0.0,1.0);
	//rim = pow(rim,rim_power);

	const float ambient_light = 0.7;

	//float light_power = ambient_light + diffuse + specular + rim;
	float light_power = ambient_light + 0.7*specular;

	//red: cubemap reflection
	//green: transparency (not used in this shader)
	//blue: not yet articulated
	vec3 misc_data = texture2D(tex_misc,v_uv_1).rgb;
	vec3 color = mix(texture2D(tex_diff,v_uv_1).rgb,ref_color,misc_data.r);

	//Height based-fog
	//Because these values never change, we can just embed them in the shader
	//Where the fog starts vertically
	float fog_height = 10.0;
	//Where the fog ends verticalls
	float fog_depth = -45.0;
	//Where the fog starts from camera (distance squared)
	float fog_start = 250.0;
	//Where the fog ends from camera (distance squared)
	float fog_end = 800.0;

	float fog_weight = 1.0 - clamp((vert_world.z - fog_depth)/(fog_height-fog_depth),0.0,1.0);
	fog_weight *= clamp((vert_world.w - fog_start)/(fog_end - fog_start),0.0,1.0);

	vec3 sky_color = textureCube(cube_map,cam_to_vert_nrmlized).xyz;
	vec3 final_color = mix(color*light_power,sky_color,fog_weight);

	gl_FragColor = vec4(final_color, 1.0);
}