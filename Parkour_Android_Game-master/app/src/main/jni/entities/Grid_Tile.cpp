//
// Created by F1 on 8/22/2016.
//

#include "Grid_Tile.hpp"

Collision_Map::Collision_Map()
{
	//Defaulting voxels to empty
	voxel[0][0] = voxel[0][1] = voxel[0][2] = voxel[0][3] = voxel[0][4] = voxel[0][5] = voxel[0][6] =
	voxel[1][0] = voxel[1][1] = voxel[1][2] = voxel[1][3] = voxel[1][4] = voxel[1][5] = voxel[1][6] =
	voxel[2][0] = voxel[2][1] = voxel[2][2] = voxel[2][3] = voxel[2][4] = voxel[2][5] = voxel[2][6] =
	voxel[3][0] = voxel[3][1] = voxel[3][2] = voxel[3][3] = voxel[3][4] = voxel[3][5] = voxel[3][6] =
	voxel[4][0] = voxel[4][1] = voxel[4][2] = voxel[4][3] = voxel[4][4] = voxel[4][5] = voxel[4][6] =
	voxel[5][0] = voxel[5][1] = voxel[5][2] = voxel[5][3] = voxel[5][4] = voxel[5][5] = voxel[5][6] =
	voxel[6][0] = voxel[6][1] = voxel[6][2] = voxel[6][3] = voxel[6][4] = voxel[6][5] = voxel[6][6] =  CLIP_EMPTY;

	//Defaulting all voxel shapes to boxes
	voxel_shape[0][0] = voxel_shape[0][1] = voxel_shape[0][2] = voxel_shape[0][3] = voxel_shape[0][4] =
	voxel_shape[0][5] = voxel_shape[0][6] = voxel_shape[1][0] = voxel_shape[1][1] = voxel_shape[1][2] =
	voxel_shape[1][3] = voxel_shape[1][4] = voxel_shape[1][5] = voxel_shape[1][6] = voxel_shape[2][0] =
	voxel_shape[2][1] = voxel_shape[2][2] = voxel_shape[2][3] = voxel_shape[2][4] = voxel_shape[2][5] =
	voxel_shape[2][6] = voxel_shape[3][0] = voxel_shape[3][1] = voxel_shape[3][2] = voxel_shape[3][3] =
	voxel_shape[3][4] = voxel_shape[3][5] = voxel_shape[3][6] = voxel_shape[4][0] = voxel_shape[4][1] =
	voxel_shape[4][2] = voxel_shape[4][3] = voxel_shape[4][4] = voxel_shape[4][5] = voxel_shape[4][6] =
	voxel_shape[5][0] = voxel_shape[5][1] = voxel_shape[5][2] = voxel_shape[5][3] = voxel_shape[5][4] =
	voxel_shape[5][5] = voxel_shape[5][6] = voxel_shape[6][0] = voxel_shape[6][1] = voxel_shape[6][2] =
	voxel_shape[6][3] = voxel_shape[6][4] = voxel_shape[6][5] = voxel_shape[6][6] =  CLIP_SHAPE_BOX;
}
Collision_Map::~Collision_Map()
{
}

Voxel Collision_Map::get_vox_at(int x, int y)
{
	return Voxel(voxel[x][y],voxel_shape[x][y]);
}

//Returns voxel[x][y] if the point (vpos_x,vpos_y) intersects with the voxel shape voxel_shape[x][y]
//the point (vpos_x,vpos_y) is assumed to be within the voxel bounds
Voxel Collision_Map::is_solid_at(int x, int y, float vpos_x, float vpos_y)
{
	return Voxel(voxel[x][y],voxel_shape[x][y]).is_solid_at(vpos_x,vpos_y);
}


Grid_Tile::Grid_Tile(int man_count, int trav_count)
{
	//model = new Static_Model("filename");
	//have to instantiate at load
	coll_map = new Collision_Map();

	maneuver_count = man_count;
	traversal_count = trav_count;
	maneuvers = new Maneuver*[man_count];
	traversals = new Traversal*[trav_count];
}
Grid_Tile::~Grid_Tile()
{
	//delete model on unload
	delete coll_map;

	for(int i = 0; i < maneuver_count; i++)
	{
		delete maneuvers[i];
	}
	for(int i = 0; i < traversal_count; i++)
	{
		delete traversals[i];
	}

	delete[] maneuvers;
	delete[] traversals;
}