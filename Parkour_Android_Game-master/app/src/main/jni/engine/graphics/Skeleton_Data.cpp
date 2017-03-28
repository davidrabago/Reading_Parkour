//
// Created by F1 on 5/31/2016.
//

#include "Skeleton_Data.hpp"

int Skeleton_Data::load(const char* filepath)
{
	raw_data = (unsigned int*) File_Utils::load_raw_asset(filepath);
	if(!raw_data)
	{
		LOGE("Error: failed to load \"%s\"\n",filepath);
		return 0;
	}

	//File Schematics
	//First int is the bone count
	//List thereafter is a list of matrices in order as they are accessed by the model
	bone_count = raw_data[0];
	rest_pose = (float*) raw_data + 1;
	return 1;
}

//Returns the rest transform of the ith bone
Mat4 Skeleton_Data::get_bone_rest_transform(int i)
{
	if(i < 0 || i >= bone_count)
	{
		LOGE("Error: tried getting rest transform of out of bounds bone index %d, bone_count=%d\n",i,bone_count);
		return Mat4::IDENTITY();
	}
	float* data = rest_pose + (i*16);
	return Mat4(data);
}

void Skeleton_Data::unload()
{
	if(raw_data)
		free((void*)raw_data);
	if(all_anims_raw_data)
	{
		for(int i = 0; i < anim_count; i++)
		{
			free((void*)all_anims_raw_data[i]);
		}
		free(all_anims_raw_data);

		free(anims);
		free(anims_IT);
		free(anim_lengths);
	}
}

Skeleton_Data::Skeleton_Data(const char *filepath)
{
	load(filepath);
}

Skeleton_Data::~Skeleton_Data()
{
	unload();
}

int Skeleton_Data::load_animation(const char* filepath)
{
	unsigned int* loaded_anim = (unsigned int*) File_Utils::load_raw_asset(filepath);

	if(!loaded_anim)
	{
		LOGE("Error: failed to load \"%s\"\n",filepath);
		return 0;
	}

	if(loaded_anim[0] != bone_count)
	{
		LOGE("Error: cannot load animation with %ud bones to a skeleton with %ud bones!\n",loaded_anim[0],bone_count);
		return 0;
	}

	anim_count++;

	//Creating new arrays that can hold all of the animations
	const unsigned int** new_anims_data = (const unsigned int**) malloc(sizeof(int*) * anim_count);
	unsigned int* new_anim_lengths = (unsigned int*) malloc(sizeof(unsigned int) * anim_count);
	float** new_anims = (float**) malloc(sizeof(float*) * anim_count);
	float** new_anims_IT = (float**) malloc(sizeof(float*) * anim_count);

	if(all_anims_raw_data && anim_lengths && anims && anims_IT)
	{
		//Copying all of the old values
		for(int i = 0; i < anim_count - 1; i++)
		{
			new_anims_data[i] = all_anims_raw_data[i];
			new_anim_lengths[i] = anim_lengths[i];
			new_anims[i] = anims[i];
			new_anims_IT[i] = anims_IT[i];
		}
		//Freeing the old arrays
		free(all_anims_raw_data);
		free(anim_lengths);
		free(anims);
		free(anims_IT);
	}
	//Handing off the array location to the class variables
	all_anims_raw_data = new_anims_data;
	anim_lengths = new_anim_lengths;
	anims = new_anims;
	anims_IT = new_anims_IT;

	int cindex = anim_count - 1;

	//File Schematics
	//First int is the bone count
	//Second int is the frame count
	//List thereafter is a list of frames
	//Each frame holds a list of data to build matrices per bone
		//This data is 16 floats for the 4x4 transform matrix
	//List thereafter is a list of frames
	//Each frame in this list holds the data needed to build inverse transpose 3x3 Matrix of the bone matrices
		//This data is 9 floats for the 3x3 matrix
	//animation_bone_count = animation_raw_data[0];

	//Assigning the new just-loaded stuff
	all_anims_raw_data[cindex] = loaded_anim;
	anim_lengths[cindex] = all_anims_raw_data[cindex][1];
	anims[cindex] = (float*) all_anims_raw_data[cindex] + 2;
	anims_IT[cindex] = (float*) all_anims_raw_data[cindex] + 2 + (16 * bone_count * anim_lengths[cindex]);
	return 1;
}
