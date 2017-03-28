//
// Created by F1 on 5/29/2016.
//

#ifndef ENGINE_SKEL_MODEL_H
#define ENGINE_SKEL_MODEL_H

#include "../common.hpp"
#include "Material.hpp"
#include "../File_Utils.hpp"
#include "Skeleton.hpp"

class Skel_Model
{
public:
	unsigned int vertex_count;
	unsigned int tri_vert_count;


	const unsigned int* raw_data = NULL;
	const float* verts;
	const float* uv_coords;
	const float* normals;
	const float* tangents;
	const float* binormals;

	const float* bone_indices;
	const float* bone_weights;

	const unsigned int* tri_verts;


	GLuint tri_verts_buffer;

	int render(Mat4 m,Mat4 vp,Material* mat, Skeleton* skel);

	int render_sans_weights(Mat4 m,Mat4 vp,Material* mat, Skeleton* skel);

	//We will eventually need methods for combining Meshes, and copying Meshes

	int init_gl();

	void term_gl();

	Skel_Model(const char* filepath);
	~Skel_Model();

private:
	int load_model(const char* filepath);
	void unload_model();

};



#endif //ENGINE_SKEL_MODEL_H
