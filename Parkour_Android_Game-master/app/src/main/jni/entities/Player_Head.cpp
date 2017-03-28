//
// Created by F1 on 4/29/2016.
//

#include "Player_Head.hpp"


void Player_Head::set_viewbob (int type)
{
	if(viewbob != type)
	{
		//Clear viewbob velocities
		viewbob_vel = Vec3::ZERO();
		viewbob_angles = Vec3::ZERO();
		//TODO: fade between viewbob types
	}
	viewbob = type;
}

void Player_Head::viewbob_run_footstep (float pitch_v,float yaw_v, float roll_v)
{
	viewbob_vel.x += pitch_v;
	viewbob_vel.y += yaw_v;
	viewbob_vel.z += roll_v;
}

void Player_Head::update_viewbob ()
{
	switch(viewbob)
	{
		case CAM_VIEWBOB_RUNNING:
		{
			//Going to simulate a spring, make the camera accelerate towards neutral
			Vec3 force(-viewbob_angles.x,-viewbob_angles.y,-viewbob_angles.z);
			//Adding resistance to the spring
			//float spring_constant = 100.0;
			float spring_constant = viewbob_spring_constant;
			//float resistance = 0.9;
			float resistance = viewbob_resistance;
			force = spring_constant*force - resistance*viewbob_vel;
			viewbob_vel = viewbob_vel + Time::udelta_time * force;
			//Don't let angles stray too far

			//float max_stray = 30.0f * DEG_TO_RAD;
			float max_stray = viewbob_max_stray * DEG_TO_RAD;

			//Damping pitch velocity
			if((viewbob_angles.x > max_stray && viewbob_vel.x > 0) || (viewbob_angles.x < -max_stray && viewbob_vel.x < 0))
			{
				viewbob_vel.x *= 0.5;
			}
			//Damping yaw velocity
			if((viewbob_angles.y > max_stray && viewbob_vel.y > 0) || (viewbob_angles.y < -max_stray && viewbob_vel.y < 0))
			{
				viewbob_vel.y *= 0.5;
			}
			//Damping roll velocity
			if((viewbob_angles.z > max_stray && viewbob_vel.z > 0) || (viewbob_angles.z < -max_stray && viewbob_vel.z < 0))
			{
				viewbob_vel.z *= 0.5;
			}

			viewbob_angles = viewbob_angles + Time::udelta_time * viewbob_vel;

			break;
		}
		case CAM_VIEWBOB_SLIDING:
			break;
		default:
		case CAM_VIEWBOB_NONE:
			break;
	}
}


//Slight modification of Entity's get_world_transform that adds in tilt_angles and viewbob_angles
Mat4 Player_Head::get_world_transform(bool modify_trans)
{
	if(transform_calculated && modify_trans)
	{
		return world_transform;
	}
	if(use_quaternion)
		transform = Mat4::ROT_TRANS(rot,pos);
	else
		transform = Mat4::ROT_TRANS(angles + tilt_angles + viewbob_angles,pos);

	if(parent)
		world_transform = parent->get_world_transform(modify_trans) * transform;
	else
		world_transform = transform;
	if(modify_trans)
		transform_calculated = true;
	return world_transform;
}
