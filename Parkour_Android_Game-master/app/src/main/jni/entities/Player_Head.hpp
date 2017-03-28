//
// Created by F1 on 4/29/2016.
//

//Player_Head is an object that the camera follows

#ifndef ENGINE_PLAYER_HEAD_H
#define ENGINE_PLAYER_HEAD_H

#include "../engine/common.hpp"
#include "../engine/misc.hpp"
#include "../engine/Game_Object.hpp"
#include "../game/game_defs.hpp"

//This class outlines an object that we use to hold the camera
//We can modify this class and add things like viewbobbing and viewangles

class Player_Head : public Entity
{
public:
	Vec3 tilt_angles;

	int viewbob;
	Vec3 viewbob_vel;

	Vec3 viewbob_angles;

	//Old values
	//float viewbob_spring_constant = 198.0f;//1000 was previous value
	//float viewbob_resistance = 2.4f;//0.9f was previous value
	//float viewbob_max_stray = 50.0f;//30 was previous value

	float viewbob_spring_constant = 380.0f;
	float viewbob_resistance = 3.5f;
	float viewbob_max_stray = 13.0f;

	void set_viewbob(int type);
	void viewbob_run_footstep(float pitch_v,float yaw_v, float roll_v);
	void update_viewbob();

	//Modification of the method derived from Entity that adds in tilt_angles and viewbob_angles
	Mat4 get_world_transform(bool modify_trans);
};

#endif //ENGINE_PLAYER_HEAD_H

