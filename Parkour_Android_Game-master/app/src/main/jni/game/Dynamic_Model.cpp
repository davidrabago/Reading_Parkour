//
// Created by F1 on 12/31/2016.
//

#include "Dynamic_Model.hpp"

int Dynamic_Model::render(Mat4 m,Mat4 vp,Material* mat)
{
	if(!gl_initialized)
	{
		return 0;
	}
	if(!mat)
	{
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
int Dynamic_Model::bind_mesh_data(Material* mat)
{
	if(!gl_initialized)
	{
		return 0;
	}
	if(mat == NULL)
	{
		return 0;
	}

	mat->bind_value(Shader::PARAM_VERTICES, (void*) verts);

	mat->bind_value(Shader::PARAM_VERT_UV1, (void*) &uv_coords_1[0]);
	mat->bind_value(Shader::PARAM_VERT_UV2, (void*) &uv_coords_2[0]);

	mat->bind_value(Shader::PARAM_VERT_NORMALS, (void*) &normals[0]);
	mat->bind_value(Shader::PARAM_VERT_TANGENTS, (void*) &tangents[0]);
	mat->bind_value(Shader::PARAM_VERT_BINORMALS, (void*) &binormals[0]);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_verts_buffer);
	return 1;
}

//Binds all data except MVP matrix and second uv coordinates
int Dynamic_Model::bind_mesh_data2(Material* mat)
{
	if(!gl_initialized)
	{
		return 0;
	}
	if(!mat)
	{
		return 0;
	}

	mat->bind_value(Shader::PARAM_VERTICES, (void*) verts);

	mat->bind_value(Shader::PARAM_VERT_UV1, (void*) uv_coords_1);

	mat->bind_value(Shader::PARAM_VERT_NORMALS, (void*) normals);
	mat->bind_value(Shader::PARAM_VERT_TANGENTS, (void*) tangents);
	mat->bind_value(Shader::PARAM_VERT_BINORMALS, (void*) binormals);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_verts_buffer);
	return 1;
}
//Assumes all data is already bound
int Dynamic_Model::render_without_bind()
{
	if(!gl_initialized)
	{
		return 0;
	}


	glDrawElements(GL_TRIANGLES, tri_vert_count, GL_UNSIGNED_INT, (void *) 0);
	return 1;
}

int Dynamic_Model::init_gl()
{
	if(gl_initialized)
	{
		return 1;
	}
	if(!model_set)
	{
		return 1;
	}

	glGenBuffers(1, &tri_verts_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, tri_verts_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (GLsizeiptr)(tri_vert_count * sizeof(unsigned int)), tri_verts, GL_STATIC_DRAW);

	gl_initialized = true;
	return 1;
}

void Dynamic_Model::term_gl()
{
	if(!gl_initialized)
	{
		return;
	}
	gl_initialized = false;
	glDeleteBuffers(1,&tri_verts_buffer);
}

//Adds a list of models together using model transforms (returns false if we ran out of room)
bool Dynamic_Model::populate_model (Static_Model **models, Mat4 *transforms, int model_count)
{
	if(model_set)
	{
		clear_model();
	}

	vertex_count = 0;
	tri_vert_count = 0;
	Vec3 vert;
	int model_verts = 0;
	int model_tri_verts = 0;
	for(int i = 0; i < model_count; i++)
	{
		if(models[i] == NULL)
		{
			LOGW("Warning: null model in dynamic model list");
			continue;
		}

		model_verts = models[i]->vertex_count;
		model_tri_verts = models[i]->tri_vert_count;

		if(vertex_count + model_verts >= DYNAMIC_MODEL_ALLOCED_VERTS)
		{
			LOGE("Warning: ran out of vertices in dynamic model. (Used: %d / %d, needed: %d) Finished %d / %d models.",vertex_count,DYNAMIC_MODEL_ALLOCED_VERTS,model_verts,i,model_count-1);
			model_set = true;
			init_gl();
			return false;
		}
		if(tri_vert_count + model_tri_verts >= DYNAMIC_MODEL_ALLOCED_TRIS)
		{
			LOGE("Warning: ran out of tri vertices in dynamic model. (Used: %d / %d, needed: %d) Finished %d / %d models.",tri_vert_count,DYNAMIC_MODEL_ALLOCED_TRIS,model_tri_verts,i,model_count-1);
			model_set = true;
			init_gl();
			return false;
		}

		int ofs_vert_index = vertex_count;
		for(int j = 0; j < model_verts; j++)
		{
			//Copying vertex positions
			vert = Vec3(models[i]->verts + 3*j);
			vert = transforms[i] * vert;
			verts[3*vertex_count] = vert.x;
			verts[3*vertex_count + 1] = vert.y;
			verts[3*vertex_count + 2] = vert.z;

			//Copying uv coords 1
			uv_coords_1[2*vertex_count] = models[i]->uv_coords_1[2*j];
			uv_coords_1[2*vertex_count + 1] = models[i]->uv_coords_1[2*j+1];
			//Copying uv coords 2
			uv_coords_2[2*vertex_count] = models[i]->uv_coords_2[2*j];
			uv_coords_2[2*vertex_count + 1] = models[i]->uv_coords_2[2*j+1];

			//I only plan on using translational transform matrices, so normals, tangents, and binormals can simply be copied over
			//Copying vertex normals
			normals[3*vertex_count] = models[i]->normals[3*j];
			normals[3*vertex_count + 1] = models[i]->normals[3*j+1];
			normals[3*vertex_count + 2] = models[i]->normals[3*j+2];
			//Copying vertex tangents
			tangents[3*vertex_count] = models[i]->tangents[3*j];
			tangents[3*vertex_count + 1] = models[i]->tangents[3*j+1];
			tangents[3*vertex_count + 2] = models[i]->tangents[3*j+2];
			//Copying vertex binormals
			binormals[3*vertex_count] = models[i]->binormals[3*j];
			binormals[3*vertex_count + 1] = models[i]->binormals[3*j+1];
			binormals[3*vertex_count + 2] = models[i]->binormals[3*j+2];

			vertex_count++;
		}
		for(int j = 0; j < model_tri_verts; j++)
		{
			tri_verts[tri_vert_count] = models[i]->tri_verts[j] + ofs_vert_index;
			tri_vert_count++;
		}
	}

	model_set = true;
	init_gl();
	return true;
}
void Dynamic_Model::clear_model()
{
	if(gl_initialized)
	{
		term_gl();
	}

	model_set = false;
	vertex_count = 0;
	tri_vert_count = 0;
}