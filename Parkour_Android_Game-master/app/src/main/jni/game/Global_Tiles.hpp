//
// Created by F1 on 9/3/2016.
//

#ifndef PARKOUR_GLOBAL_TILES_HPP
#define PARKOUR_GLOBAL_TILES_HPP

#include "../entities/Grid_Tile.hpp"
#include "Interior_Variant.hpp"
#include "Window_Model_Holder.hpp"
#include "../engine/graphics/Skel_Model.hpp"


//Tile Terminology
//
//Style: the entire tile-set models that makes up a style of interior
//Style Variant: different textures/materials that can be applied to a tile set
//TODO: if we go with procedural colors, variants will be different textures with different features, and we will randomly choose colors to use in a shader for variety
//Type: the general navigation type of a tile
//		i.e. this tile is empty, this tile is a solid block (blocks all navigation), this tile is a wall
//			this tile has an obstacle on it, etc...
//Tile Subtype: different models with potentially different traversals of a tile type

//General routine:
//
//	Building picks an exterior style and a random exterior variant
//	Building picks an interior style, and a random interior variant (if random colors, choose those)
//	Floor begins generation and populates tile types 2d array
//		This evaluates what tile types will go where
//	Floor then chooses a random variant of each tile type for each tile in the floor

//How variant materials / textures are going to be stored
//
//	Interior_Style holds a list of Interior_Variant
//		This class holds methods for randomizing shader color values
//		This class is responsible for the color palette generation
//		To use the material, we just call bind to bind the material
//		A method called gen_palette to make random colors
//
//	Exterior Style holds a list of Exterior_Variant
//		Exterior_Style holds window models
//		Exterior_Variant holds window shaders and window inside and outside textures
//		To use a specific variant, we just call bind on the variant

//
//Fallback: if palette generation fails, we can store a large array of preset interior palettes and just use those
//


class Interior_Style
{
public:
	//Holds materials,diffuse, normal, and lightmap textures for this Interior Style
	Interior_Variant* variants[1];

	//TODO: use tiles
	Grid_Tile** tiles[TILE_TYPES];
	//Holds the number of variants per type (also the length of each array pointed to by tiles pointer array)
	int type_variant_counts[TILE_TYPES];


	//For now, holding explicit references to the tiles
	Grid_Tile* empty_tile;
	//Wall tiles:
	Grid_Tile* wall_subtypes[WALL_TYPE_COUNT] = {NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL,NULL};
	//Rail tiles:
	Grid_Tile* rail_subtypes[RAIL_TYPE_COUNT];

	//Obstacle Tiles
	Grid_Tile* obst_tiles[10];


	Interior_Style();
	~Interior_Style();
};

class Exterior_Style
{
public:
	//Holds materials,diffuse, normal, and lightmap textures for this Interior Style
	Exterior_Variant *variants[1];

	Window_Model_Holder* window_models;
	Interior_Window_Model_Holder* int_window_models;

	//One Instance of broken windows per style
	Skel_Model* broken_out_window;
	Skeleton_Data* broken_out_window_skel_data;

	Skel_Model* broken_in_window;
	Skeleton_Data* broken_in_window_skel_data;

	Exterior_Style();
	~Exterior_Style();
};

class Global_Tiles
{
public:
	static Global_Tiles* instance;

	static int init_data();
	static void term_data();

	Interior_Style* tile_styles[1];

	Exterior_Style* window_styles[1];

	//Used for both skybox and building exterior reflections
	Cube_Map* sky_cube_map = NULL;

	//Global Building to building traversals (referenced by player)
	Traversal* bldg_travs[3];

	Global_Tiles();
	~Global_Tiles();
	static void init_gl();
	static void term_gl();
};

#endif //PARKOUR_GLOBAL_TILES_HPP
