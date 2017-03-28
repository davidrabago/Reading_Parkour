//
// Created by F1 on 1/13/2017.
//

#ifndef PARKOUR_SKELETON_ENTITY_HPP
#define PARKOUR_SKELETON_ENTITY_HPP

#include "Skeleton_Data.hpp"


//This class is an entity that uses the asset Skeleton
class Skeleton : public Entity
{
public:
	Skeleton_Data* skel_data;

	unsigned int bone_count = 0;

	float* current_pose_mat4s;
	float* current_pose_mat3s;

	float* rest_pose_ident_mat4s;
	float* rest_pose_ident_mat3s;

	bool lerp_anim = true;
	float frame_time = ANIM_FPS_60;

	int set_fps(float fps);

	bool playing_anim = false;
	//Whether the currently playing animation is animating (used for pausing an animation)
	bool animating = false;
	int current_frame = 0;
	int dest_frame = 0;
	int current_anim = -1;
	int current_anim_end_type = ANIM_END_TYPE_ROOT_POSE;
	float time_for_next_frame = 0.0f;

	int last_frames_passed[ANIM_MAX_SKIPPABLE_FRAMES];
	int last_frames_passed_anims[ANIM_MAX_SKIPPABLE_FRAMES];
	int last_frames_passed_count = 0;


	bool playing_default_anim = false;
	int default_anim = -1;
	int default_anim_end_type = ANIM_END_TYPE_ROOT_POSE;

	int set_default_anim(int anim, int end_type);

	int anim_is_valid(int anim);

	int play_anim(int anim, int end_type);

	int play_default_anim();

	int stop_anim();

	int pause_anim();
	int resume_anim();

	//Ran every frame to update animation frame logic (calculate interpolation data, increment frame, etc)
	int update_frame();

	void calc_lerped_pose_mats();
	void calc_pose_mats();

	//Returns a pointer to the current frame matrices
	float* get_current_pose();

	//Returns a pointer to the current frame inverse-transpose matrices
	float* get_current_pose_IT();

	//Returns the ith bone's current transform matrix (within animation)
	Mat4 get_bone_transform(int i);
	//Returns the rest transform of the ith bone
	Mat4 get_bone_rest_transform(int i);

	Skeleton(Skeleton_Data* skeleton_data);
	~Skeleton();
};


//Helper class for parenting an Entity to a skeleton bone
class Entity_Bone_Joint : public Game_Object
{
public:
	Skeleton* parent_skel = NULL;
	int parent_bone_index = 0;

	Mat4 get_world_transform(bool modify_trans);
};



#endif //PARKOUR_SKELETON_ENTITY_HPP
