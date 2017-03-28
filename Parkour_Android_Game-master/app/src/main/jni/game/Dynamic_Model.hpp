//
// Created by F1 on 12/31/2016.
//

#ifndef PARKOUR_DYNAMIC_MODEL_HPP
#define PARKOUR_DYNAMIC_MODEL_HPP

//This is a test class extension of Static_Model, meant to be a model that is allowed to be modified at runtime
//This differs from static model in that it comes with a huge pre-allocated array of memory, so that we do not need to allocate at runtime.

#include "../engine/common.hpp"
#include "../engine/graphics/Material.hpp"
#include "../engine/graphics/Static_Model.hpp"
#include "../engine/File_Utils.hpp"
#include "../engine/math/math.hpp"

#define DYNAMIC_MODEL_ALLOCED_VERTS 800000
#define DYNAMIC_MODEL_ALLOCED_TRIS 800000

class Dynamic_Model
{
public:
	bool gl_initialized = false;
	bool model_set = false;

	unsigned int vertex_count = 0;
	unsigned int tri_vert_count = 0;

	float verts[DYNAMIC_MODEL_ALLOCED_VERTS*3];
	float uv_coords_1[DYNAMIC_MODEL_ALLOCED_VERTS*2];
	float uv_coords_2[DYNAMIC_MODEL_ALLOCED_VERTS*2];
	float normals[DYNAMIC_MODEL_ALLOCED_VERTS*3];
	float tangents[DYNAMIC_MODEL_ALLOCED_VERTS*3];
	float binormals[DYNAMIC_MODEL_ALLOCED_VERTS*3];

	unsigned int tri_verts[DYNAMIC_MODEL_ALLOCED_TRIS*3];

	GLuint tri_verts_buffer;

	int render(Mat4 m,Mat4 vp,Material* mat);

	//Binds all data except MVP matrix
	int bind_mesh_data(Material* mat);

	//Binds all data except MVP matrix and second uv coordinates
	int bind_mesh_data2(Material* mat);

	//Assumes all data is already bound
	int render_without_bind();


	int init_gl();

	void term_gl();

	//Trivial constructors and destructors are adequate
	Dynamic_Model() {};
	~Dynamic_Model () {};

	//Combines n static models together
	bool populate_model(Static_Model** models, Mat4* transforms, int model_count);
	void clear_model();
};

#endif //PARKOUR_DYNAMIC_MODEL_HPP
