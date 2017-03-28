//
// Created by F1 on 5/31/2016.
//

#ifndef ENGINE_SKELETON_H
#define ENGINE_SKELETON_H
#include "../common.hpp"
#include "../File_Utils.hpp"
#include "../Game_Object.hpp"
#include "../misc.hpp"

class Skeleton_Data
{
public:
	unsigned int bone_count = 0;
	float* rest_pose;

	const unsigned int* raw_data = NULL;
	//Arrays of pointers or values for all animations
	const unsigned int** all_anims_raw_data = NULL;
	unsigned int* anim_lengths;
	//Array of transform 4x4 matrices representing all animation frame transformations
	float** anims;
	//Array of transform 3x3 rotation matrices representing all inverse-transpose of bone transform
	float** anims_IT;

	int anim_count = 0;

	//Returns the rest transform of the ith bone
	Mat4 get_bone_rest_transform(int i);

	int load_animation(const char* filepath);

	Skeleton_Data(const char* filepath);
	~Skeleton_Data();

private:
	int load(const char* filepath);
	void unload();
};



#endif //ENGINE_SKELETON_H
