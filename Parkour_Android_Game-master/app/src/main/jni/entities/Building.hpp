//
// Created by F1 on 8/22/2016.
//

#ifndef PARKOUR_BUILDING_HPP
#define PARKOUR_BUILDING_HPP

#include "../engine/math/math.hpp"
#include "../game/game_defs.hpp"
#include "Floor.hpp"

class Building : public Entity
{
public:
	//Pos of the building is going to be defined as (smallest(closest to 0),smallest(closest to 0),bottom of building)
	//pos is near left bottom of building

	//Size of the building in meters
	Vec3 size;
	//Size of the building in tiles (ints)
	//x & y are in floor tiles, z is in windowgridtiles
	Vec3 dimensions;
	//Global left near bottom of building
	Vec3 global_mins;
	//Global right far top of building
	Vec3 global_maxs;
	//Number of floors this building has
	int floors = 0;

	//Index of the window broken out of the building
	int broken_owindow_index_x = 0;
	int broken_owindow_index_y = 0;

	//Index of the window broken into the building
	int broken_iwindow_index_x = 0;
	int broken_iwindow_index_y = 0;

	bool broken_iwindow_active = false;
	Skeleton* broken_iwindow_skel = NULL;
	bool broken_owindow_active = false;
	Skeleton* broken_owindow_skel = NULL;

	//char tile_types[MAX_WIDTH][MAX_LENGTH];

	bool generated = false;
	Floor *active_floor = NULL;
	int active_floor_number = 0;

	//Dispatches to floor's input_to_maneuver
	//	ultimately returns a maneuver if there exists a maneuver in the current floor's tileset such that:
	//	(the input required to start the maneuver is input_type) AND (the player is within the bounding box required to start the maneuver)
	//	returns NULL if no such Maneuver exists
	Maneuver* input_to_maneuver(Vec3 pos, int input_type, Vec3& tile_ofs);

	//Dispatches to floor's input_to_traversal
	//	ultimately returns a traversal if we are at the last tile of the building and:
	//	(input_type is the input required to start the traversal) AND (the player is within the bounding box required to start the traversal)
	//	returns NULL if no such traversal exists
	Traversal* input_to_traversal(Vec3 pos, int input_type, Vec3& tile_ofs);


	Vec3 get_tile_ofs_at_pos(Vec3 p);


	Building();
	~Building();

	//Creates the building
	void generate(Building* prev_bldg,Vec3 bldg_ofs);

	void generate_floor(Vec3 player_pos, Building* next_bldg);

	//Currently regenerates the building using different rng numbers
	void regenerate_floor(Vec3 player_pos, Building* next_bldg);

	//Clears the created building, zeroes everything out
	void clear();

	//Returns the floor's voxel at point p
	Voxel get_voxel_at(Vec3 p);
	//Returns if the floor has a voxel that is solid at the point p
	char is_solid_at(Vec3 p);

	//Returns true if the point p is out of the building's inner bounds, false if not
	bool is_out_of_bounds(Vec3 p);

	//Returns true if the point p is in the building, or within 5 m of the front of the building
	bool is_in_bounds_or_near_front(Vec3 p);

	//Returns true if any corner of the bounding box 2*half_width x 2*half_width located at point p is out of bounds
	bool is_box_out_of_bounds(Vec3 p, float half_width);

	//Returns Vec3 in bounds
	Vec3 clamp_to_bounds(Vec3 p);


	//Holds a list of shell models for the building exterior
	//Different walls are kept in different lists because there
	// are times when the front wall and back wall need to be regenerated

	//Front wall models:
	Static_Model* ext_mdls_fw[BUILDING_MAX_EXTERIOR_MODELS];
	Mat4 ext_mdl_fw_trans[BUILDING_MAX_EXTERIOR_MODELS];
	int ext_mdl_fw_count = 0;

	//Back wall models:
	Static_Model* ext_mdls_bw[BUILDING_MAX_EXTERIOR_MODELS];
	Mat4 ext_mdl_bw_trans[BUILDING_MAX_EXTERIOR_MODELS];
	int ext_mdl_bw_count = 0;

	//Left wall models:
	Static_Model* ext_mdls_lw[BUILDING_MAX_EXTERIOR_MODELS];
	Mat4 ext_mdl_lw_trans[BUILDING_MAX_EXTERIOR_MODELS];
	int ext_mdl_lw_count = 0;

	//Right wall models:
	Static_Model* ext_mdls_rw[BUILDING_MAX_EXTERIOR_MODELS];
	Mat4 ext_mdl_rw_trans[BUILDING_MAX_EXTERIOR_MODELS];
	int ext_mdl_rw_count = 0;


	//Holds a list of shell models for the building interior

	//Front wall models:
	Static_Model* int_mdls_fw[BUILDING_MAX_INTERIOR_MODELS];
	Mat4 int_mdl_fw_trans[BUILDING_MAX_INTERIOR_MODELS];
	int int_mdl_fw_count = 0;

	//Front wall models:
	Static_Model* int_mdls_bw[BUILDING_MAX_INTERIOR_MODELS];
	Mat4 int_mdl_bw_trans[BUILDING_MAX_INTERIOR_MODELS];
	int int_mdl_bw_count = 0;

	//Left wall models:
	Static_Model* int_mdls_lw[BUILDING_MAX_INTERIOR_MODELS];
	Mat4 int_mdl_lw_trans[BUILDING_MAX_INTERIOR_MODELS];
	int int_mdl_lw_count = 0;

	//Right wall models:
	Static_Model* int_mdls_rw[BUILDING_MAX_INTERIOR_MODELS];
	Mat4 int_mdl_rw_trans[BUILDING_MAX_INTERIOR_MODELS];
	int int_mdl_rw_count = 0;



//#define DEBUG_SUBDIVIDE_WALL

	//Recursively 2d partitions a wall into the largest (nxn) (n being 2^i for all integers i in [0,5]) tiles that fit
	//Then we draw these tiles, drastically reducing the draw call counts for the buildings
	void subdivide_wall(Mat4 trans, int wall_width, int wall_height, int* mdl_count, Static_Model **mdl_list, Mat4* trans_list);

	//Subdivide an interior window wall horizontally
	void subdivide_interior_wall(Mat4 trans, int wall_width, int *mdl_count, Static_Model **mdl_list, Mat4 *trans_list);

	void generate_exterior_model_list();

	void generate_interior_model_list();


	int render_ext_walls(Mat4 vp);

	int render_int_walls(Mat4 vp);

	int render(Vec3 player_pos, Mat4 vp);

	//Rendering method called at the end to render transparent windows and tiles
	int render_transparent_meshes(Vec3 player_pos, Mat4 vp);

	//Sets the window that player_pos hits as broken. (direction: true = into the building, false = out of the building)
	void break_window(Vec3 player_pos, bool into_building);

	//Returns the floor number p is in
	int get_floor_num_at_pos(Vec3 p);

	//Offsets the building and all of the building's data
	void offset_building(Vec3 ofs);

	//Updated any game logic that the building may need to handle
	void update();
};
#endif //PARKOUR_BUILDING_HPP
