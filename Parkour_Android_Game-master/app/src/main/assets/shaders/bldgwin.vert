uniform mat4 mvp;
uniform mat4 m;

uniform mat3 m_IT;

attribute vec4 vert_pos;
attribute vec2 vert_uv_1;
attribute vec3 vert_nor;
attribute vec3 vert_tan;
attribute vec3 vert_binor;

uniform vec3 cam_pos;
uniform vec3 cam_dir;
uniform vec3 dirlight_dir;

varying vec2 v_uv_1;
varying vec3 cam_dir_tanspace;
varying vec3 dirlight_dir_tanspace;
varying vec3 cam_to_vert_tanspace;
varying mat3 tan_to_world;

//xyz world coords of vert, w is distance to the camera squared
varying vec4 vert_world;
//Normalized vector from camera to vertex
varying vec3 cam_to_vert_nrmlized;

void main()
{
	vec4 pos;
	pos = vec4(vert_pos.xyz,1.0);
	gl_Position = mvp * pos;
	vec3 world_vert = (m * pos).xyz;

	vert_world.xyz = world_vert;
	//Calculating the camera direction and light direction in tangent space
	//These 3 vectors are in world space
	vec3 v_nor = m_IT * vert_nor;
	vec3 v_tan = m_IT * vert_tan;
	vec3 v_binor = m_IT * vert_binor;

	mat3 temp = mat3(v_tan,v_binor,v_nor);
	tan_to_world = temp;
	//Manually transposing the matrix... (Does GLES2 not have a transpose function?)
	mat3 world_to_tangent = mat3(temp[0][0],temp[1][0],temp[2][0],
							temp[0][1],temp[1][1],temp[2][1],
							temp[0][2],temp[1][2],temp[2][2]);

	dirlight_dir_tanspace = world_to_tangent * -dirlight_dir;
	cam_dir_tanspace = world_to_tangent * -cam_dir;

	//Calculating cubemap reflection view direction
	//Getting length between camera and vert squared
	vert_world.w = dot(world_vert - cam_pos,world_vert - cam_pos);
	cam_to_vert_nrmlized = normalize(world_vert - cam_pos);
	cam_to_vert_tanspace = normalize(world_to_tangent * (world_vert - cam_pos));

	v_uv_1 = vert_uv_1;
}