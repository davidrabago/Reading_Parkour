//
// Created by F1 on 1/13/2017.
//

#include "Skeleton.hpp"



//TODO: on instantiation, populate list of rest poses.
//Populating a list of identity matrices for displaying the skeleton's rest post
/*
Mat4 ident4 = Mat4::IDENTITY();
rest_pose_ident_mat4s = (float*) malloc(sizeof(float) * bone_count*16);
for(int i = 0; i < 16; i++)
{
for(int j = 0; j < bone_count; j++)
{
rest_pose_ident_mat4s[j*16 + i] = ident4.m[i];
}
}
Mat3 ident3 = Mat3::IDENTITY();
rest_pose_ident_mat3s = (float*) malloc(sizeof(float) * bone_count * 9);
for(int i = 0; i < 9; i++)
{
for(int j = 0; j < bone_count; j++)
{
rest_pose_ident_mat3s[j*9 + i] = ident3.m[i];
}
}
*/
//================================================================================================================================

int Skeleton::set_fps(float fps)
{
	frame_time = 1.0f/fps;
	return 1;
}

int Skeleton::set_default_anim(int anim, int end_type)
{
	if(!anim_is_valid(anim))
	{
		LOGE("Error: tried setting animation to %d when only %d animations are loaded by this skeleton.\n",anim,skel_data->anim_count);
		return 0;
	}
	default_anim = anim;
	default_anim_end_type = end_type;
	return 1;
}

int Skeleton::anim_is_valid(int anim)
{
	if(anim <0 || anim > skel_data->anim_count)
		return 0;
	return 1;
}

int Skeleton::play_anim(int anim, int end_type)
{
	if(!anim_is_valid(anim))
	{
		LOGE("Error: tried setting animation to %d when only %d animations are loaded by this skeleton.\n",anim,skel_data->anim_count);
		return 0;
	}
	current_anim_end_type = end_type;
	current_anim = anim;
	time_for_next_frame = 0.0f;
	current_frame = 0;
	dest_frame = 1;
	playing_anim = true;
	animating = true;
	return 1;
}

int Skeleton::play_default_anim()
{
	if(!anim_is_valid(default_anim))
	{
		LOGE("Error: tried playing invalid default animation %d.",default_anim);
		return 0;
	}
	playing_anim = true;
	playing_default_anim = true;
	animating = true;
	current_anim = default_anim;
	time_for_next_frame = 0.0f;
	current_frame = 0;
	dest_frame = 1;
	current_anim_end_type = default_anim_end_type;
	return 1;
}

int Skeleton::stop_anim()
{
	playing_anim = false;
	animating = false;
	current_anim = -1;
	current_anim_end_type = ANIM_END_TYPE_ROOT_POSE;
	time_for_next_frame = 0.0f;
	current_frame = 0;
	dest_frame = 0;
	return 1;
}

int Skeleton::pause_anim()
{
	animating = false;
	return 1;
}

int Skeleton::resume_anim()
{
	animating = true;
	time_for_next_frame = 0.0f;
	return 1;
}

//Calculate pose matrices somewhere between current_frame and dest_frame
void Skeleton::calc_lerped_pose_mats()
{
	//Getting lerping factor:
	float ctime = Time::time();
	float t = 1.0f - (time_for_next_frame - ctime)/frame_time;
	t = fmaxf(fminf(t,1.0f),0.0f);

	//Repopulating our current frame matrices with the new correct bone matrices
	Mat4 bone_trans;
	Mat3 bone_IT_trans;
	float* trans_a;
	float* trans_b;
	float* trans_IT_a;
	float* trans_IT_b;

	for(int i = 0; i < bone_count; i++)
	{
		//Calculate transformation Mat4
		trans_a = (skel_data->anims[current_anim]) + (bone_count * 16 * current_frame) + (i*16);
		trans_b = (skel_data->anims[current_anim]) + (bone_count * 16 * dest_frame) + (i*16);

		bone_trans = Mat4::LERP(Mat4(trans_a),Mat4(trans_b),t);
		//Copying transformation Mat4 into the float array
		for(int j = 0; j < 16; j++)
		{
			current_pose_mat4s[16*i + j] = bone_trans.m[j];
		}

		//Calculate inverse-transpose Mat3
		trans_IT_a = (skel_data->anims_IT[current_anim]) + (bone_count * 9 * current_frame) + (i*9);
		trans_IT_b = (skel_data->anims_IT[current_anim]) + (bone_count * 9 * dest_frame) + (i*9);

		bone_IT_trans = Mat3::LERP(Mat3(trans_IT_a),Mat3(trans_IT_b),t);
		//Copying inverse-transpose Mat3 into the float array
		for(int j = 0; j < 9; j++)
		{
			current_pose_mat3s[9*i + j] = bone_IT_trans.m[j];
		}
	}
}

//Calculate at current frame (no lerp)
void Skeleton::calc_pose_mats()
{
	//Repopulating our current frame matrices with the new correct bone matrices
	Mat4 bone_trans;
	Mat3 bone_IT_trans;
	float* trans;
	float* trans_IT;

	for(int i = 0; i < bone_count; i++)
	{
		//Calculate transformation Mat4

		trans = (skel_data->anims[current_anim]) + (bone_count * 16 * current_frame) + (i*16);
		bone_trans = Mat4(trans);
		//Copying transformation Mat4 into the float array
		for(int j = 0; j < 16; j++)
		{
			current_pose_mat4s[16*i + j] = bone_trans.m[j];
		}


		//Calculate inverse-transpose Mat3
		trans_IT = (skel_data->anims_IT[current_anim]) + (bone_count * 9 * current_frame) + (i*9);
		bone_IT_trans = Mat3(trans_IT);

		//Copying inverse-transpose Mat3 into the float array
		for(int j = 0; j < 9; j++)
		{
			current_pose_mat3s[9*i + j] = bone_IT_trans.m[j];
		}
	}
}

//Ran every frame to update animation frame logic (calculate interpolation data, increment frame, etc)
int Skeleton::update_frame()
{
	last_frames_passed_count = 0;
	if(!playing_anim || !animating)
		return 1;

	float ctime = Time::time();

	//If time_for_next_frame is 0, we initialize it to ctime
	if(time_for_next_frame == 0.0f)
		time_for_next_frame = ctime;

	//If we are 5 or more frames behind, something has gone terribly wrong, so just ignore those missing frames
	if((ctime - time_for_next_frame) / frame_time >= 5.0f)
		ctime = time_for_next_frame;


	//We don't want to miss a frame over an extremely small margin, check +/- 5 milliseconds
	while(ctime > time_for_next_frame - 0.001f && last_frames_passed_count < ANIM_MAX_SKIPPABLE_FRAMES)
	{
		current_frame += 1;
		dest_frame += 1;
		//time_for_next_frame = ctime + frame_time;
		time_for_next_frame += frame_time;
		if(current_frame >= skel_data->anim_lengths[current_anim])
		{
			switch(current_anim_end_type)
			{
				case ANIM_END_TYPE_ROOT_POSE:
				default:
					stop_anim();
					return 1;
				case ANIM_END_TYPE_FREEZE:
					pause_anim();
					return 1;
				case ANIM_END_TYPE_LOOP:
					current_frame = 0;
					dest_frame = 1;
					break;
				case ANIM_END_TYPE_DEFAULT_ANIM:
					if(anim_is_valid(default_anim))
					{
						play_default_anim();
					}
					else
					{
						LOGW("Warning: anim end type of play_default_anim was invoked with an invalid default anim of %d\n",default_anim);
						stop_anim();
						return 1;
					}
					break;
			}
		}

		last_frames_passed[last_frames_passed_count] = current_frame;
		last_frames_passed_anims[last_frames_passed_count] = current_anim;
		last_frames_passed_count++;

		//Setting which frame to lerp to
		if(dest_frame >= skel_data->anim_lengths[current_anim])
		{
			switch(current_anim_end_type)
			{
				case ANIM_END_TYPE_ROOT_POSE:
				default:
				case ANIM_END_TYPE_FREEZE:
					dest_frame--;
					break;
				case ANIM_END_TYPE_LOOP:
					dest_frame = 0;
					break;
				case ANIM_END_TYPE_DEFAULT_ANIM:
					//Technically next frame is going to be frame 0 of default anim... I sense complications here
					//Note: this doesn't lerp, I refuse to implement animation blending
					//(based on my implementation of a mesh's skeleton,
					// 		....each bone would lerp independently of every other bone,
					// 		this would be a nightmare)
					dest_frame = current_frame;//no lerp
					break;
			}
		}
	}

	if(current_frame != dest_frame)
		calc_lerped_pose_mats();
	else
		calc_pose_mats();

	return 1;
}


//Returns a pointer to the current frame matrix generation data
float* Skeleton::get_current_pose()
{
	if(!playing_anim)
	{
		return rest_pose_ident_mat4s;
	}
	return current_pose_mat4s;
}

//Returns a pointer to the current frame's inverse-transpose matrix generation data
float* Skeleton::get_current_pose_IT()
{
	if(!playing_anim)
	{
		return rest_pose_ident_mat3s;
	}
	return current_pose_mat3s;
}


//Returns the ith bone's current transform matrix generation data (within animation)
Mat4 Skeleton::get_bone_transform(int i)
{
	if(!playing_anim)
	{
		return Mat4::IDENTITY();
	}
	if(i < 0 || i >= bone_count)
	{
		LOGE("Error: tried getting transform of out of bounds bone index %d, bone_count=%d\n",i,bone_count);
		return Mat4::IDENTITY();
	}

	float* data = current_pose_mat4s + (i*16);
	return Mat4(data);
}


//Returns the rest transform of the ith bone
Mat4 Skeleton::get_bone_rest_transform(int i)
{
	if(i < 0 || i >= bone_count)
	{
		LOGE("Error: tried getting rest transform of out of bounds bone index %d, bone_count=%d\n",i,bone_count);
		return Mat4::IDENTITY();
	}
	float* data = skel_data->rest_pose + (i*16);
	return Mat4(data);
}


Skeleton::Skeleton(Skeleton_Data* skeleton_data)
{
	skel_data = skeleton_data;
	bone_count = skel_data->bone_count;
	time_for_next_frame = 0.0f;

	//Populating a list of identity matrices for displaying the skeleton's rest post
	Mat4 ident4 = Mat4::IDENTITY();
	rest_pose_ident_mat4s = (float*) malloc(sizeof(float) * bone_count*16);
	current_pose_mat4s = (float*) malloc(sizeof(float) * bone_count*16);
	for(int i = 0; i < 16; i++)
	{
		for(int j = 0; j < bone_count; j++)
		{
			rest_pose_ident_mat4s[j*16 + i] = ident4.m[i];
			//Initializing current pose mat4s to identity matrices
			current_pose_mat4s[j*16 + i] = ident4.m[i];
		}
	}
	Mat3 ident3 = Mat3::IDENTITY();
	rest_pose_ident_mat3s = (float*) malloc(sizeof(float) * bone_count * 9);
	current_pose_mat3s = (float*) malloc(sizeof(float) * bone_count * 9);
	for(int i = 0; i < 9; i++)
	{
		for(int j = 0; j < bone_count; j++)
		{
			rest_pose_ident_mat3s[j*9 + i] = ident3.m[i];
			//Initializing current pose mat3s to identity matrices
			current_pose_mat3s[j*9 + i] = ident3.m[i];
		}
	}
}

Skeleton::~Skeleton()
{
	if(rest_pose_ident_mat4s)
		free(rest_pose_ident_mat4s);
	if(rest_pose_ident_mat3s)
		free(rest_pose_ident_mat3s);
	if(current_pose_mat4s)
		free(current_pose_mat4s);
	if(current_pose_mat3s)
		free(current_pose_mat3s);
}


//================================================================================================================================
Mat4 Entity_Bone_Joint::get_world_transform(bool modify_trans)
{
	if(!parent_skel)
	{
		LOGE("Error: Entity_Bone_Joint skeletal parent not set.\n");
		return Mat4::IDENTITY();
	}
	if(transform_calculated && modify_trans)
	{
		return world_transform;
	}

	transform = parent_skel->get_bone_transform(parent_bone_index) * parent_skel->get_bone_rest_transform(parent_bone_index);

	if(modify_trans)
		transform_calculated = true;
	//Bone transforms seem to introduce a roll of 90 degrees, so undoing it
	Quat fix_roll(HALF_PI, Vec3::FRONT());

	return parent_skel->get_world_transform(modify_trans) * transform * Mat4::ROTATE(fix_roll);
}