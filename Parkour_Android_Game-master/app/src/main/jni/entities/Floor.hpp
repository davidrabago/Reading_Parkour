//
// Created by F1 on 8/22/2016.
//

#ifndef PARKOUR_FLOOR_HPP
#define PARKOUR_FLOOR_HPP

#include "../engine/math/math.hpp"
#include "../engine/Game_Object.hpp"
#include "Grid_Tile.hpp"
#include "../game/Global_Tiles.hpp"
#include "../game/Dynamic_Model.hpp"

class Floor : Entity
{
public:
	//height of the floor's ground level
	float altitude;

	//Width/length in tiles
	int width;
	int length;

	//Pos is defined as the global_mins of the floor
	Vec3 global_pos;
	Vec3 global_mins;
	Vec3 global_maxs;

	//Has the floor been generated?
	bool generated = false;

	//Type index of tile
	int tile_type[BUILDING_MAX_WIDTH][BUILDING_MAX_LENGTH];
	//Subtype index of tile (used for walls)
	int tile_subtype[BUILDING_MAX_WIDTH][BUILDING_MAX_LENGTH];
	//Variant index of tile
	int tile_variant[BUILDING_MAX_WIDTH][BUILDING_MAX_LENGTH];

	//Temp matrix that we're going to use to generate the branching path of the player
	int tile_branch_type[BUILDING_MAX_WIDTH][BUILDING_MAX_LENGTH];

	//What tile set are we using?
	int interior_style;
	//What material are we using?
	int interior_variant;

	//For convenience/speed, we're going to hold an array of pointers to models and collision maps
	Collision_Map* tile_coll_map[BUILDING_MAX_WIDTH][BUILDING_MAX_LENGTH];
	Static_Model* tile_model[BUILDING_MAX_WIDTH][BUILDING_MAX_LENGTH];
	Grid_Tile* tile_object[BUILDING_MAX_WIDTH][BUILDING_MAX_LENGTH];

	Dynamic_Model* dynamic_floor_model;

	int goal_min_column = 0;
	int goal_max_column = 0;

	//Temporary array of Vec3s to render as lines
	Vec3 branch_debug_points[3000];
	//Every 2 points (Vec3) is a line
	int branch_debug_point_count = 0;
	Material* debug_branch_mat = NULL;

	Floor();
	~Floor() { }

	void clear_floor_tiles();


	//Populates the floor matrices reading generated type, subtypes, and variants
	void populate_floor();

	struct Wall
	{
		char x1 = 0;
		char y1 = 0;
		char x2 = 0;
		char y2 = 0;

		Wall()
		{
			x1 = y1 = x2 = y2 = 0;
		}
		Wall(const char a,const char b,const char c,const char d)
		{
			x1 = a;
			y1 = b;
			x2 = c;
			y2 = d;
		}
	};

	struct Room
	{
		char min_x = 0;
		char min_y = 0;
		char max_x = 0;
		char max_y = 0;

		Room()
		{
			min_x = min_y = max_x = max_y = 0;
		}

		Room(const char a,const char b,const char c,const char d)
		{
			min_x = a;
			min_y = b;
			max_x = c;
			max_y = d;
		}

		Wall get_north_wall()
		{
			return Wall(min_x,max_y,max_x,max_y);
		}

		Wall get_east_wall()
		{
			return Wall(max_x,min_y,max_x,max_y);
		}

		Wall get_south_wall()
		{
			return Wall(min_x,min_y,max_x,min_y);
		}

		Wall get_west_wall()
		{
			return Wall(min_x,min_y,min_x,max_y);
		}
	};

//#define DEBUG_RBSP

	//Recursively divides the room **stack_ptr, returns the divided rooms on the array *stack_ptr
	void recursive_bsp(Room** stack_ptr,const bool horizontal_divide);

	//Returns true if min <= x <= max
	inline bool in_bounds(const char x,const char min,const char max);

	//Returns true if a == b == c == d
	inline bool all_equal(const char a,const char b,const char c,const char d);
	//Checks if w1 is completely inside of w2
	//0 means walls are not inside each other
	//1 means w1 is inside of w2
	//2 means w2 is inisde of w1
	char is_wall_in_wall(const Wall* w1,const Wall* w2);

	//Adds wall to list if w is not in any wall on the list
	//Replaces a wall in the list if w is completely inside of the wall and smaller
	//Does not add w to the list if there is a wall in the list completely inside w and smaller than w
	//Does not add w to the list if the wall makes up the building exterior
	//Returns 0 if wall was not added, returns 1 otherwise
	int add_wall_to_list(const Wall* w, Wall* list_start, Wall** list_end);

	//Returns unique walls given a list of rooms
	//Where rooms is a pointer to Rooms, last_room is a pointer to the last room
	//Walls is a pointer to an array of enough walls
	void get_unique_walls(Wall** wall_ptr, Room* rooms,const Room* last_room);

	void set_vert_wall_tiles(Wall* w);

	void set_hor_wall_tiles(Wall* w);

	//Branches left from tile[tile_x][tile_y]
	void recursive_branch_left(int tile_x, int tile_y);

	//Branches right from tile[tile_x][tile_y]
	void recursive_branch_right(int tile_x, int tile_y);

	//Branches forward from tile[tile_x][tile_y]
	void recursive_branch_forward(int tile_x, int tile_y);

	float prob_of_branch_left = 0.2f;
	float prob_of_branch_right = 0.2f;
	float prob_of_branch_forward = 0.5f;

	//Probability that branch left given that we just branched right
	float prob_of_branch_right_given_branch_left = 0.3f;
	//Probability that branch right given that we just branched left
	float prob_of_branch_left_given_branch_right = 0.3f;

	//Returns probability that we need to branch towards goal_column
	//Returns 0.0f - 1.0f, or any value > 1.0f if it's impossible to get to the goal tile
	float prob_of_branch_to_goal(int tile_x, int tile_y);

//#define DEBUG_BRANCH_LOGIC

	// General layout of the logic of this recursive method
		//============
		//1: Check if we're at the end of the building, or if this tile has already branched (terminating case)
		//============
		//2: Perform some tests that prohibit us from branching in certain directions
			//These are tests that cannot be passed with modifications to the wall layout
		//============
		//3: Perform additional tests that halt us from branching
			//These are tests that can be passed if we modify the wall layout
		//============
		//4: Calculate probabilities that we must branch left or right in order to get between gmin_x and gmax_x
		//============
		//5: Finally, analyze what directions we can still branch in, and perform the recursive branches
		//============
		//6: if we cannot branch, modify the wall layout in order to pass one of the failed tests from section 3
		//============
	void recursive_branch_player_path(int tile_x, int tile_y, int prev_branch);
	//Sets the tile at [x][y] as a rail tile
	//If the tile is not a rail, sets it's subtype as rail_type
	//If the tile was already a rail, adds rail_type to it's subtype
	void set_rail_type(int x, int y, int rail_type);

	void place_rail_tiles();

	void generate(Vec3 p, int floor_num, Vec3 mins, Vec3 maxs,Vec3 dims, Vec3 player_pos, int _goal_min_column, int _goal_max_column);


	int render(Mat4 vp);

	void clear();

	//Assumes point p is defined relative to global_mins
	//Returns the voxel at point p
	//if the point p is out of bounds, returns voxel clip type CLIP_WINDOW, and clip shape as either COL_DIR_FORWARD or COL_DIR_RIGHT
	//depending on whether or not the out of bounds occured on the x-axis or y-axis
	Voxel get_voxel_at(Vec3 p);
	//Returns if the floor has a voxel that is solid at the point p
	//Assumes p is relative to global_mins
	char is_solid_at(Vec3 p);

	//Is the global y coord out of the floor bounds?
	bool is_y_out_of_bounds(Vec3 p);
	//Is the global x coord out of the floor bounds?
	bool is_x_out_of_bounds(Vec3 p);
	//Is the y coord (relative to our min) out of the floor bounds?
	bool is_local_y_out_of_bounds(Vec3 p);
	//Is the x coord (relative to our min) out of the floor bounds?
	bool is_local_x_out_of_bounds(Vec3 p);


	//	Returns a maneuver if there exists a maneuver in the this floor's tileset such that:
	//	(the input required to start the maneuver is input_type) AND (the player is within the bounding box required to start the maneuver)
	//	returns NULL if no such tile exists

	//==== Quick note on what tiles we have to check ====
	//Previously, the 1st keyframe of a maneuver had to lie within that tile
	//But that is no longer the case

	//As a rule of thumb, the start of the maneuver must lie within the tile itself, or the tile before it
	//so for a given player position, we must check the tile the player is on and the tile after it (+1 in y direction)
	//===================================================

	//p is the global position
	Maneuver* input_to_maneuver(Vec3 p, int input_type, Vec3& tile_ofs);


	//	Returns a traversal if there exists a traversal if pos is within the last tile, and if:
	//	(the input required to start the traversal is input_type) AND (the player is within the bounding box required to start the traversal)
	//	returns NULL otherwise
	//Assigns tile_ofs to the world position of whatever tile the maneuver is on
	Traversal* input_to_traversal(Vec3 p, int input_type, Vec3& tile_ofs);
	//Returns the global tile position that p lies on
	Vec3 get_tile_ofs_at_pos(Vec3 p);
};

#endif //PARKOUR_FLOOR_HPP
