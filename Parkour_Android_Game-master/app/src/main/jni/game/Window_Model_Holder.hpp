//
// Created by F1 on 11/21/2016.
//

#ifndef PARKOUR_WINDOW_MESH_HOLDER_HPP
#define PARKOUR_WINDOW_MESH_HOLDER_HPP

#include "../engine/graphics/Static_Model.hpp"
#include "game_defs.hpp"

class Window_Model_Holder
{
public:
	Static_Model* tile_model = NULL;
	Static_Model* m2x2_model = NULL;
	Static_Model* m4x4_model = NULL;
	Static_Model* m8x8_model = NULL;
	Static_Model* m16x16_model = NULL;
	Static_Model* m32x32_model = NULL;

	//Horizontal strips
	Static_Model* m1x2_model = NULL;
	Static_Model* m1x4_model = NULL;
	Static_Model* m1x8_model = NULL;
	Static_Model* m1x16_model = NULL;
	Static_Model* m1x32_model = NULL;

	//Vertical strips
	Static_Model* m2x1_model = NULL;
	Static_Model* m4x1_model = NULL;
	Static_Model* m8x1_model = NULL;
	Static_Model* m16x1_model = NULL;
	Static_Model* m32x1_model = NULL;

	Window_Model_Holder(const char* filepath);

	~Window_Model_Holder ();

	int init_gl();
	int term_gl();
};

//This class is for only holding horizontal tiles
class Interior_Window_Model_Holder
{
public:
	Static_Model* tile_model = NULL;
	//Horizontal strips
	Static_Model* m1x2_model = NULL;
	Static_Model* m1x4_model = NULL;
	Static_Model* m1x8_model = NULL;
	Static_Model* m1x16_model = NULL;
	Static_Model* m1x32_model = NULL;

	Interior_Window_Model_Holder(const char* filepath);

	~Interior_Window_Model_Holder ();

	int init_gl();
	int term_gl();
};

#endif //PARKOUR_WINDOW_MESH_HOLDER_HPP
