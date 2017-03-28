//
// Created by F1 on 5/5/2016.
//

#ifndef ENGINE_STATIC_MODEL_H
#define ENGINE_STATIC_MODEL_H

#include "../common.hpp"
#include "Material.hpp"
#include "../File_Utils.hpp"
#include "../math/math.hpp"
class Static_Model
{
public:
	unsigned int vertex_count;
	unsigned int tri_vert_count;

	const unsigned int* raw_data = NULL;
	const float* verts;
	const float* uv_coords_1;
	const float* uv_coords_2;
	const float* normals;
	const float* tangents;
	const float* binormals;

	const unsigned int* tri_verts;

	GLuint tri_verts_buffer;

	int render(Mat4 m,Mat4 vp,Material* mat);

	//Binds all data except MVP matrix
	int bind_mesh_data(Material* mat);

	//Binds all data except MVP matrix and second uv coordinates
	int bind_mesh_data2(Material* mat);

	//Assumes all data is already bound
	int render_without_bind();


	//We will eventually need methods for combining Meshes, and copying Meshes
	int init_gl();

	void term_gl();

	//Copies the other static model
	Static_Model(Static_Model* other);
	//Combines two static models together
	Static_Model(Static_Model* model1, Mat4 model1_trans, Static_Model* model2, Mat4 model2_trans);
	//Loads static model from file
	Static_Model(const char* filepath);
	~Static_Model();
private:
	int load_model(const char* filepath);
	void unload_model();

	void parse_raw_data();
};

#endif //ENGINE_STATIC_MODEL_H
