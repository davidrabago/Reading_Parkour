//
// Created by F1 on 4/29/2016.
//

#ifndef ENGINE_PLAYER_H
#define ENGINE_PLAYER_H

#include "../engine/Game_Object.hpp"
#include "../engine/graphics/Skel_Model.hpp"

class Player : public Entity
{
public:
	static const int ANIM_RUN = 1;
	static const int ANIM_SHOWCASE_HANDS = 2;
	static const int ANIM_SPEEDVAULT = 3;
	static const int ANIM_JUMP = 4;

	Skeleton* skel;
	//Have to store an array of all models that make up the player
	Skel_Model* player_model1;
	Skel_Model* player_model2;
	Skel_Model* player_model3;

	Material* mat1;
	Material* mat2;
	Material* mat3;

	int render(Mat4 vp);

	//Update method that is ran every frame
	int update();
};

#endif //ENGINE_PLAYER_H
