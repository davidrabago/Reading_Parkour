//
// Created by F1 on 8/22/2016.
//

#ifndef PARKOUR_TILE_HPP
#define PARKOUR_TILE_HPP


#include "../engine/graphics/Static_Model.hpp"
#include "../game/Maneuver.hpp"
#include "../game/game_defs.hpp"
#include "../engine/misc.hpp"

//Used for returning voxel types
struct Voxel
{
	char clip_type = CLIP_SOLID;
	char clip_shape = CLIP_SHAPE_BOX;

	Voxel()
	{
	}

	Voxel(char type, char shape)
	{
		clip_type = type;
		clip_shape = shape;
	}

	//Defaults to box
	Voxel(char type)
	{
		clip_type = type;
		clip_shape = CLIP_SHAPE_BOX;
	}

	//Returns clip_type if point (px,py) intersect with clip_shape
	//It is assumed px lies in [0,GRID_SIZE], and py lies in [0,GRID_SIZE]
	char is_solid_at(float px, float py)
	{
		switch(clip_shape)
		{
			default:
			case CLIP_SHAPE_BOX:
				return clip_type;

				//Solid area is the area under the line py=px
			case CLIP_SHAPE_LT_POS:
				return (py <= px) ? clip_type : (char)CLIP_EMPTY;

				//Solid area is the area above the line py=px
			case CLIP_SHAPE_GT_POS:
				return (py >= px) ? clip_type : (char)CLIP_EMPTY;
				//Solid area is the area under the line py=0.5 - px
			case CLIP_SHAPE_LT_NEG:
				return (py <= (0.5f - px)) ? clip_type : (char)CLIP_EMPTY;

				//Solid area is the area above the line py=(px-0.25) + 0.25
			case CLIP_SHAPE_GT_NEG:
				return (py >= (0.5f - px)) ? clip_type : (char)CLIP_EMPTY;

				//Solid area is the area under the line py=abs(px-0.25) + 0.25
			case CLIP_SHAPE_GT_ABS:
				return (py <= (fabsf(px-0.25f) + 0.25f)) ? clip_type : (char)CLIP_EMPTY;

				//Solid area is the area above the line py=-abs(px-0.25) + 0.25
			case CLIP_SHAPE_LT_ABS:
				return (py >= (-fabsf(px-0.25f) + 0.25f)) ? clip_type : (char)CLIP_EMPTY;

				//Solid area is the area above the line py=px-0.15 and under the line py=px+0.15
			case CLIP_SHAPE_IN_WALL_POS:
				return ( (py >= px - 0.15f) && (py <= px + 0.15f) ) ? clip_type : (char)CLIP_EMPTY;

				//Solid area is the area above the line py=-px+0.35 and under the line py=-px+0.65
			case CLIP_SHAPE_IN_WALL_NEG:
				return ( (py >= -px + 0.35f) && (py <= -px + 0.65f) ) ? clip_type : (char)CLIP_EMPTY;
		}
	}
};

class Collision_Map
{
public:
	char voxel[TILE_VOXEL_DIMS][TILE_VOXEL_DIMS];
	char voxel_shape[TILE_VOXEL_DIMS][TILE_VOXEL_DIMS];

	Collision_Map();
	~Collision_Map();

	//Returns voxel[x][y] if the point (vpos_x,vpos_y) intersects with the voxel shape voxel_shape[x][y]
	//the point (vpos_x,vpos_y) is assumed to be within the voxel bounds
	Voxel is_solid_at(int x, int y, float vpos_x, float vpos_y);
	Voxel get_vox_at(int x, int y);
};

class Grid_Tile
{
public:
	//TODO: store maneuver data
	//TODO: store traversal data
	Maneuver** maneuvers;
	int maneuver_count;

	Traversal** traversals;
	int traversal_count;

	Static_Model* model;
	Collision_Map* coll_map;

	Grid_Tile(int man_count, int trav_count);
	~Grid_Tile();
};

#endif //PARKOUR_TILE_HPP
