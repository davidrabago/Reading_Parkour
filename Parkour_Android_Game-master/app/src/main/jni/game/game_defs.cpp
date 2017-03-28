//
// Created by F1 on 12/11/2016.
//

#include "game_defs.hpp"

//This file holds non-preprocessor variable definitions


//Array of vertex shapes of voxel shapes.
int CLIP_SHAPE_VERT_COUNTS[CLIP_SHAPE_COUNT] = {4,3,3,3,3,3,3,6,6};

//These arrays hold the 2D vertices that make up the voxel shapes
float CLIP_SHAPE_VERTS_BOX[] = 	{0,0,		0,GRID_SIZE,			GRID_SIZE,0,		GRID_SIZE,GRID_SIZE};
float CLIP_SHAPE_VERTS_LT_POS[] = 	{0,0,		GRID_SIZE,0,			GRID_SIZE,GRID_SIZE};
float CLIP_SHAPE_VERTS_GT_POS[] = 	{0,0,		0,GRID_SIZE,			GRID_SIZE,GRID_SIZE};
float CLIP_SHAPE_VERTS_LT_NEG[] = 	{0,0,		0,GRID_SIZE,			GRID_SIZE,0};
float CLIP_SHAPE_VERTS_GT_NEG[] = 	{0,GRID_SIZE,	GRID_SIZE,0,			GRID_SIZE,GRID_SIZE};
float CLIP_SHAPE_VERTS_LT_ABS[] = 	{0,0,		GRID_SIZE,0,			0.5f*GRID_SIZE,0.5f*GRID_SIZE};
float CLIP_SHAPE_VERTS_GT_ABS[] = 	{0,GRID_SIZE,	GRID_SIZE,GRID_SIZE,	0.5f*GRID_SIZE,0.5f*GRID_SIZE};;
float CLIP_SHAPE_VERTS_IN_WALL_POS[] =
{
	0,0,					0,0.15f,					0.15f,0,
	GRID_SIZE,GRID_SIZE,	GRID_SIZE-0.15f,GRID_SIZE,	GRID_SIZE,GRID_SIZE-0.15f
};
float CLIP_SHAPE_VERTS_IN_WALL_NEG[] =
{
	0,GRID_SIZE,			0,GRID_SIZE-0.15f,			0.15f,GRID_SIZE,
	GRID_SIZE,0,			GRID_SIZE-0.15f,0,			GRID_SIZE,0.15f
};

float* CLIP_SHAPE_SHAPES[CLIP_SHAPE_COUNT] =
{
	CLIP_SHAPE_VERTS_BOX,
	CLIP_SHAPE_VERTS_LT_POS,
	CLIP_SHAPE_VERTS_GT_POS,
	CLIP_SHAPE_VERTS_LT_NEG,
	CLIP_SHAPE_VERTS_GT_NEG,
	CLIP_SHAPE_VERTS_GT_ABS,
	CLIP_SHAPE_VERTS_LT_ABS,
	CLIP_SHAPE_VERTS_IN_WALL_POS,
	CLIP_SHAPE_VERTS_IN_WALL_NEG
};



//Value is the index of the building that precedes the building whose index we are accessing
int PREV_BLDG[3] = {2,0,1};
//Value is the index of the building that follows the building whose index we are accessing
int NEXT_BLDG[3] = {1,2,0};

//Array whose entries are values of valid rail types
bool RAIL_TYPE_IS_VALID[RAIL_TYPE_COUNT] = {1, 1, 1, 1, 1, 1, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 1, 0, 1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};



//Array used for filtering out unwanted characters from keyboard input
//Here we filter out all control characters, except for ' ','\b', and '\n'. (keeps all symbolic & alphanumeric)
char INPUT_CHAR_FILTER[128] = {0,0,0,0,0,0,0,0,8/*\b*/,0,10/*\n*/,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,32/*' '*/,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,0/*DEL*/};
//Unmodified filter array: {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,52,53,54,55,56,57,58,59,60,61,62,63,64,65,66,67,68,69,70,71,72,73,74,75,76,77,78,79,80,81,82,83,84,85,86,87,88,89,90,91,92,93,94,95,96,97,98,99,100,101,102,103,104,105,106,107,108,109,110,111,112,113,114,115,116,117,118,119,120,121,122,123,124,125,126,127}