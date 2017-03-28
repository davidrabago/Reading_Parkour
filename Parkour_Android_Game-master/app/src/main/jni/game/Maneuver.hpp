//
// Created by F1 on 9/19/2016.
//

#ifndef PARKOUR_MANEUVER_HPP
#define PARKOUR_MANEUVER_HPP

#include "../engine/common.hpp"
#include "game_defs.hpp"

class Keyframe
{
public:
	Keyframe()
	{
		mins = Vec3::ZERO();
		maxs = Vec3::ZERO();
	}
	//Boundary for executing keyframe
	Vec3 mins;
	Vec3 maxs;

	//Y velocity for the frame
	float y_vel = 0;//y speed for keyframe
	float y_accel = 0;//y axis acceleration (for speeding up or slowing down)
	float min_y_vel = 0;//minimum y axis speed

	//y-axis lerp type
	int speed_type = FRAME_LERP_LINEAR;

	//z-axis interpolation types
	int lerp_type = FRAME_LERP_LINEAR;//interpolation used for z-axis (spherical, linear, quadratic, inverse quadratic)
	float lerp_data = 0;



	//Turn the player towards a direction
	int orient = FRAME_ORIENT_NONE;
	Vec3 orient_pos;//x & y coordinates of point that player faces as we head towards keyframe
	//How interpolation factor ( 0 < factor <= 1 )
	float orient_speed = PLAYER_TURN_LERP_FACTOR;

	//Animation to play starting at this keyframe
	//What cmd to execute
	int anim_cmd = FRAME_ANIM_NOOP;
	//Which animation (only used if anim_cmd is FRAME_ANIM_PLAY)
	int anim = 0;
	//How to end the animation played (only used if anim_cmd is FRAME_ANIM_PLAY)
	int anim_end_type = ANIM_END_TYPE_ROOT_POSE;
	int viewbob_type = CAM_VIEWBOB_NONE;

	//Special keyframe data (for letting the player know to do special things)
	int spec_flag = FRAME_SPECFLAG_NONE;

	void set_bounds(Vec3 _mins, Vec3 _maxs)
	{
		mins = _mins;
		maxs = _maxs;
	}
	//Set bounds as a single point where mins = maxs
	void set_bounds(Vec3 point)
	{
		mins = point;
		maxs = point;
	}
	void set_speed(float _y_vel, float _y_accel, float _min_y_vel)
	{
		y_vel = _y_vel;
		y_accel = _y_accel;
		min_y_vel = _min_y_vel;
	}
	void set_speed(float _y_vel, int _speed_type)
	{
		y_vel = _y_vel;
		speed_type = _speed_type;
	}

	//Sets lerp type for frame, lerp_data is z acceleration for LERP_QUADRATIC, is not used otherwise
	void set_lerp(int _lerp_type, float _lerp_data)
	{
		lerp_type = _lerp_type;
		lerp_data = _lerp_data;
	}

	void set_orient(int _orient, Vec3 _orient_pos, float _orient_speed)
	{
		orient = _orient;
		orient_pos = _orient_pos;
		orient_speed = _orient_speed;
	}

	void set_anim(int _anim_cmd, int _anim, int _anim_end_type)
	{
		anim_cmd = _anim_cmd;
		anim = _anim;
		anim_end_type = _anim_end_type;
	}
	//Alternate set_anim, for setting anim_cmd (there are many commands that don't require the extra params)
	void set_anim(int _anim_cmd)
	{
		anim_cmd = _anim_cmd;
	}

	void set_vbob(int _viewbob_type)
	{
		viewbob_type = _viewbob_type;
	}

	void set_specflag(int _spec_flag)
	{
		spec_flag = _spec_flag;
	}


};

class Maneuver
{
public:
	Keyframe** keyframes;
	int keyframe_count = 0;
	int input_required = INPUT_SWIPE_NONE;

	//Regardless of where we are, if we are on the last frame of the animation, stop the maneuver when the current animation ends
	bool end_on_anim_end = true;

	Maneuver( int frame_count)
	{
		keyframe_count = frame_count;
		keyframes = new Keyframe*[frame_count];
		for(int i = 0; i < frame_count; i++)
		{
			keyframes[i] = new Keyframe();
		}
	}

	~Maneuver()
	{
		for(int i = 0; i < keyframe_count; i++)
		{
			delete keyframes[i];
		}
		delete[] keyframes;
	}

	void set_input(int _input_required)
	{
		input_required = _input_required;
	}
};

class Traversal : public Maneuver
{
public:
	//TODO: what misc data is required for traversal?
	Traversal(int frame_count) : Maneuver(frame_count)
	{

	}
};

#endif //PARKOUR_MANEUVER_HPP
