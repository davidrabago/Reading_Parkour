//
// Created by F1 on 3/29/2016.
//

#include "math.hpp"



Vec3 Vec3::angles_to_forward() const
{
	Vec3 frwd;

	Quat pitch(x, Vec3::RIGHT());
	//Rotating angles.y about UP vector for yaw
	Quat yaw(y, Vec3::UP());

	Quat rot = yaw*pitch;
	frwd = rot * Vec3::FRONT();

	return frwd;
}
//Returns right vector from angles
Vec3 Vec3::angles_to_right() const
{
	Vec3 right;
	Quat pitch(x, Vec3::RIGHT());
	//Rotating angles.y about UP vector for yaw
	Quat yaw(y, Vec3::UP());

	Quat rot = yaw*pitch;
	Vec3 forward = rot * Vec3::FRONT();

	//Rotating angles.z about the forward vector for the roll
	Quat roll(z, forward);
	//Adding roll to pitch * yaw
	rot = roll * rot;
	right = rot * Vec3::RIGHT();
	return right;
}
//Returns up angle from vector
Vec3 Vec3::angles_to_up() const
{
	Vec3 up;
	Quat pitch(x, Vec3::RIGHT());
	//Rotating angles.y about UP vector for yaw
	Quat yaw(y, Vec3::UP());

	Quat rot = yaw*pitch;
	Vec3 forward = rot * Vec3::FRONT();

	//Rotating angles.z about the forward vector for the roll
	Quat roll(z, forward);
	//Adding roll to pitch * yaw
	rot = roll * rot;
	up = rot * Vec3::UP();
	return up;
}

//Returns forward, right, and up vector from angles
//This is done for convenience since many calculations overlap when doing any of these by themselves
void Vec3::angles_to_dirs(Vec3 *forward, Vec3 *right, Vec3 *up) const
{
	Quat pitch(x, Vec3::RIGHT());
	//Rotating angles.y about UP vector for yaw
	Quat yaw(y, Vec3::UP());

	Quat rot = yaw*pitch;
	*forward = rot * Vec3::FRONT();

	//Rotating angles.z about the forward vector for the roll
	Quat roll(z, *forward);
	//Adding roll to pitch * yaw
	rot = roll * rot;
	*right = rot * Vec3::RIGHT();
	*up = Vec3::cross(*right,*forward);
}