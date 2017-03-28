//
// Created by F1 on 5/5/2016.
//

#include "Static_Model.hpp"
int Static_Model::render(Mat4 m,Mat4 vp,Material* mat)
{
	if(!mat)
	{
		LOGW("Warning: tried rendering a static model without assigning material\n");
		return 0;
	}

	mat->bind_value(Shader::PARAM_VERTICES, (void*) verts);

	mat->bind_value(Shader::PARAM_VERT_UV1, (void*) uv_coords_1);
	mat->bind_value(Shader::PARAM_VERT_UV2, (void*) uv_coords_2);

	mat->bind_value(Shader::PARAM_VERT_NORMALS, (void*) normals);
	mat->bind_value(Shader::PARAM_VERT_TANGENTS, (void*) tangents);
	mat->bind_value(Shader::PARAM_VERT_BINORMALS, (void*) binormals);

	Mat4 mvp = vp * m;
	mat->bind_value(Shader::PARAM_MVP_MATRIX, (void*) mvp.m);

	Mat3 m_it = m.inverted_then_transposed().get_mat3();
	mat->bind_value(Shader::PARAM_M_IT_MATRIX, (void*) m_it.m);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_verts_buffer);
	glDrawElements(GL_TRIANGLES, tri_vert_count, GL_UNSIGNED_INT, (void *) 0);

	return 1;
}

//Binds all data except MVP matrix
int Static_Model::bind_mesh_data(Material* mat)
{
	if(!mat)
	{
		LOGW("Warning: tried rendering a static model without assigning material\n");
		return 0;
	}

	mat->bind_value(Shader::PARAM_VERTICES, (void*) verts);

	mat->bind_value(Shader::PARAM_VERT_UV1, (void*) uv_coords_1);
	mat->bind_value(Shader::PARAM_VERT_UV2, (void*) uv_coords_2);

	mat->bind_value(Shader::PARAM_VERT_NORMALS, (void*) normals);
	mat->bind_value(Shader::PARAM_VERT_TANGENTS, (void*) tangents);
	mat->bind_value(Shader::PARAM_VERT_BINORMALS, (void*) binormals);

	//Mat4 mvp = vp * m;
	//mat->bind_value(Shader::PARAM_MVP_MATRIX, (void*) mvp.m);

	//Mat3 m_it = m.inverted_then_transposed().get_mat3();
	//mat->bind_value(Shader::PARAM_M_IT_MATRIX, (void*) m_it.m);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_verts_buffer);
}

//Binds all data except MVP matrix and second uv coordinates
int Static_Model::bind_mesh_data2(Material* mat)
{
	if(!mat)
	{
		LOGW("Warning: tried rendering a static model without assigning material\n");
		return 0;
	}

	mat->bind_value(Shader::PARAM_VERTICES, (void*) verts);

	mat->bind_value(Shader::PARAM_VERT_UV1, (void*) uv_coords_1);

	mat->bind_value(Shader::PARAM_VERT_NORMALS, (void*) normals);
	mat->bind_value(Shader::PARAM_VERT_TANGENTS, (void*) tangents);
	mat->bind_value(Shader::PARAM_VERT_BINORMALS, (void*) binormals);

	//Mat4 mvp = vp * m;
	//mat->bind_value(Shader::PARAM_MVP_MATRIX, (void*) mvp.m);

	//Mat3 m_it = m.inverted_then_transposed().get_mat3();
	//mat->bind_value(Shader::PARAM_M_IT_MATRIX, (void*) m_it.m);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_verts_buffer);
}
//Assumes all data is already bound
int Static_Model::render_without_bind()
{
	glDrawElements(GL_TRIANGLES, tri_vert_count, GL_UNSIGNED_INT, (void *) 0);
	return 1;
}

void Static_Model::parse_raw_data()
{
	//File Schematics
	//First int is the vertex count
	//Second int is the triangle count * 3
	//List thereafter is the position (3 floats) of all vertices
	//List thereafter is the uv tex coords of the 1st uv map (2 floats) of all vertices
	//List thereafter is the uv tex coords of the 2nd uv map (2 floats) of all vertices
	//List thereafter is the normals (3 floats) of all vertices
	//List thereafter is the tangents (3 floats) of all vertices
	//List thereafter is the binormals (3 floats) of all vertices
	//List thereafter is the indices of vertices that make up the triangles
	vertex_count = raw_data[0];
	tri_vert_count = raw_data[1];

	verts = (float*) (raw_data + 2);

	uv_coords_1 = (float*) raw_data + 2 + (vertex_count*3);
	uv_coords_2 = (float*) raw_data + 2 + (vertex_count*3) + (vertex_count*2);

	normals = (float*) raw_data + 2 + (vertex_count*3) + 2*(vertex_count*2);
	tangents = (float*) raw_data + 2 + 2*(vertex_count*3) + 2*(vertex_count*2);
	binormals = (float*) raw_data + 2 + 3*(vertex_count*3) + 2*(vertex_count*2);

	tri_verts = raw_data + 2 + 4*(vertex_count*3) + 2*(vertex_count*2);
}

int Static_Model::load_model(const char* filepath)
{
	raw_data = (unsigned int*) File_Utils::load_raw_asset(filepath);
	if(!raw_data)
	{
		LOGE("Error: failed to load \"%s\"\n",filepath);
		return 0;
	}

	parse_raw_data();

	return 1;
}

int Static_Model::init_gl()
{
	glGenBuffers(1, &tri_verts_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_verts_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(tri_vert_count * sizeof(unsigned int)), tri_verts, GL_STATIC_DRAW);

	return 1;
}

void Static_Model::term_gl()
{
	glDeleteBuffers(1,&tri_verts_buffer);
}
void Static_Model::unload_model()
{
	if(raw_data)
		free((void*)raw_data);
}
//Loads a static model from file
Static_Model::Static_Model(const char* filepath)
{
	load_model(filepath);
}
//Copies the other static model
Static_Model::Static_Model(Static_Model* other)
{
	int raw_data_size = 2 + (16 * other->vertex_count) + other->tri_vert_count;

	unsigned int* temp_raw_data = NULL;
	temp_raw_data = (unsigned int*) malloc(sizeof(int) * raw_data_size);

	if(!temp_raw_data)
	{
		LOGE("Error: failed to allocate memory for raw data when copying a static model");
		return;
	}

	//Copying this the other model's raw data
	for(int i = 0; i < raw_data_size; i++)
	{
		temp_raw_data[i] = other->raw_data[i];
	}
	raw_data = temp_raw_data;

	parse_raw_data();
}
//Combines two static models together
Static_Model::Static_Model(Static_Model* model1, Mat4 model1_trans, Static_Model* model2, Mat4 model2_trans)
{
	//Allocate enough room for both meshes
	int raw_data_size = 2 + ((16 * model1->vertex_count) + model1->tri_vert_count) + ((16 * model2->vertex_count) + model2->tri_vert_count);

	//Though the raw data is a int pointer, most attributes are floats, so I'll make this a float pointer to avoid multiple casts
	float* temp_raw_data = NULL;
	temp_raw_data = (float*) malloc(sizeof(float) * raw_data_size);

	if(!temp_raw_data)
	{
		LOGE("Error: failed to allocate memory for raw data when combining two static models");
		return;
	}

	unsigned int m1_vert_count = model1->vertex_count;
	unsigned int m2_vert_count = model2->vertex_count;
	unsigned int new_vert_count = m1_vert_count + m2_vert_count;

	unsigned int m1_tri_vert_count = model1->tri_vert_count;
	unsigned int m2_tri_vert_count = model2->tri_vert_count;

	//============================================ Setting up raw model data ====================================================

	//Assignining vertex count
	((unsigned int*)temp_raw_data)[0] = m1_vert_count + m2_vert_count;
	//Assigning triangle vertices count
	((unsigned int*)temp_raw_data)[1] = model1->tri_vert_count + model2->tri_vert_count;

	Vec3 temp_v;

	//==== Copying over the vertex attributes ====
	//Disclaimer: I know the transformations for the meshes will be isometric,
	// therefore taking the inverse-transpose of the model transform matrices is unnecessary for the normal/tangent/binormal calculation

	Mat3 model1_nortrans = model1_trans.inverted_then_transposed().get_mat3();
	Mat3 model2_nortrans = model2_trans.inverted_then_transposed().get_mat3();

	//Copying vertex attributes from model 1
	for(unsigned int i = 0; i < m1_vert_count; i++)
	{
		//Vertex position
		temp_v = Vec3(model1->verts + i*3);
		temp_v = model1_trans * temp_v;
		temp_raw_data[2 + 3*i] = temp_v.x;
		temp_raw_data[2 + 3*i + 1] = temp_v.y;
		temp_raw_data[2 + 3*i + 2] = temp_v.z;

		//UV 1 coords
		temp_raw_data[2 + (3*new_vert_count) + 2*i] = model1->uv_coords_1[i*2];
		temp_raw_data[2 + (3*new_vert_count) + 2*i + 1] = model1->uv_coords_1[i*2 + 1];
		//UV 2 coords
		temp_raw_data[2 + (5*new_vert_count) + 2*i] = model1->uv_coords_2[i*2];
		temp_raw_data[2 + (5*new_vert_count) + 2*i + 1] = model1->uv_coords_2[i*2 + 1];

		//Normals
		temp_v = Vec3(model1->normals + i*3);
		temp_v = model1_nortrans * temp_v;
		temp_raw_data[2 + (7*new_vert_count) + 3*i] = temp_v.x;
		temp_raw_data[2 + (7*new_vert_count) + 3*i + 1] = temp_v.y;
		temp_raw_data[2 + (7*new_vert_count) + 3*i + 2] = temp_v.z;

		//Tangents
		temp_v = Vec3(model1->tangents + i*3);
		temp_v = model1_nortrans * temp_v;
		temp_raw_data[2 + (10*new_vert_count) + 3*i] = temp_v.x;
		temp_raw_data[2 + (10*new_vert_count) + 3*i + 1] = temp_v.y;
		temp_raw_data[2 + (10*new_vert_count) + 3*i + 2] = temp_v.z;

		//Binormals
		temp_v = Vec3(model1->binormals + i*3);
		temp_v = model1_nortrans * temp_v;
		temp_raw_data[2 + (13*new_vert_count) + 3*i] = temp_v.x;
		temp_raw_data[2 + (13*new_vert_count) + 3*i + 1] = temp_v.y;
		temp_raw_data[2 + (13*new_vert_count) + 3*i + 2] = temp_v.z;
	}
	//Copying vertex attributes from model 2
	for(unsigned int i = 0; i < m2_vert_count; i++)
	{
		//Vertex position
		temp_v = Vec3(model2->verts + i*3);
		temp_v = model2_trans * temp_v;
		temp_raw_data[2 + 3*i + (3*m1_vert_count)] = temp_v.x;
		temp_raw_data[2 + 3*i + (3*m1_vert_count) + 1] = temp_v.y;
		temp_raw_data[2 + 3*i + (3*m1_vert_count) + 2] = temp_v.z;

		//UV 1 coords
		temp_raw_data[2 + (3*new_vert_count) + (2*m1_vert_count) + 2*i] = model2->uv_coords_1[i*2];
		temp_raw_data[2 + (3*new_vert_count) + (2*m1_vert_count) + 2*i + 1] = model2->uv_coords_1[i*2 + 1];
		//UV 2 coords
		temp_raw_data[2 + (5*new_vert_count) + (2*m1_vert_count) + 2*i] = model2->uv_coords_2[i*2];
		temp_raw_data[2 + (5*new_vert_count) + (2*m1_vert_count) + 2*i + 1] = model2->uv_coords_2[i*2 + 1];

		//Normals
		temp_v = Vec3(model2->normals + i*3);
		temp_v = model2_nortrans * temp_v;
		temp_raw_data[2 + (7*new_vert_count) + (3*m1_vert_count) + 3*i] = temp_v.x;
		temp_raw_data[2 + (7*new_vert_count) + (3*m1_vert_count) + 3*i + 1] = temp_v.y;
		temp_raw_data[2 + (7*new_vert_count) + (3*m1_vert_count) + 3*i + 2] = temp_v.z;

		//Tangents
		temp_v = Vec3(model2->tangents + i*3);
		temp_v = model2_nortrans * temp_v;
		temp_raw_data[2 + (10*new_vert_count) + (3*m1_vert_count) + 3*i] = temp_v.x;
		temp_raw_data[2 + (10*new_vert_count) + (3*m1_vert_count) + 3*i + 1] = temp_v.y;
		temp_raw_data[2 + (10*new_vert_count) + (3*m1_vert_count) + 3*i + 2] = temp_v.z;

		//Binormals
		temp_v = Vec3(model2->binormals + i*3);
		temp_v = model2_nortrans * temp_v;
		temp_raw_data[2 + (13*new_vert_count) + (3*m1_vert_count) + 3*i] = temp_v.x;
		temp_raw_data[2 + (13*new_vert_count) + (3*m1_vert_count) + 3*i + 1] = temp_v.y;
		temp_raw_data[2 + (13*new_vert_count) + (3*m1_vert_count) + 3*i + 2] = temp_v.z;
	}

	//===== Copying triangle vertex indices =====
	//Copying triangle vertex indices from model 1
	for(unsigned int i = 0; i < m1_tri_vert_count; i++)
	{
		((unsigned int*)temp_raw_data)[2 + (16*new_vert_count) + i] = model1->tri_verts[i];
	}
	//Copying triangle vertex indices from model 2
	for(unsigned int i = 0; i < m2_tri_vert_count; i++)
	{
		((unsigned int*)temp_raw_data)[2 + (16*new_vert_count) + m1_tri_vert_count + i] = m1_vert_count + model2->tri_verts[i];
	}

	//==============================================================================================================

	raw_data = (unsigned int*) temp_raw_data;

	parse_raw_data();
}
Static_Model::~Static_Model()
{
	unload_model();
}