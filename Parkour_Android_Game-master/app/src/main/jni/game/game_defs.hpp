//
// Created by F1 on 10/12/2016.
//

#ifndef PARKOUR_GAME_DEFS_HPP

#include "../engine/engine_defs.hpp"

//========================= Tile Defs =====================================

//Total count of tile types
#define TILE_TYPES 2

#define TILE_TYPE_EMPT 0
#define TILE_TYPE_WALL 1
#define TILE_TYPE_RAIL 2
//FIXME: this next one is temp
#define TILE_TYPE_OBST 3

#define TILE_OBSTACLE_COUNT 10

//Collision map Voxel Clip Types
//Not solid
#define CLIP_EMPTY 0
//Full wall
#define CLIP_SOLID 1
//Obstacle that's empty low to the ground
#define CLIP_SLIDE 2
//Obstacle that we can collide with if we're jumping
#define CLIP_DOORWAY 3
//Waist-high obstacle
#define CLIP_MID 4

//Building breakable window (not used for collision, but for death animation type)
#define CLIP_WINDOW 5


//Defining a few different clip shapes
#define CLIP_SHAPE_BOX 0
//Solid area is the area under the line y=x
#define CLIP_SHAPE_LT_POS 1
//Solid area is the area above the line y=x
#define CLIP_SHAPE_GT_POS 2
//Solid area is the area under the line y=0.5-x
#define CLIP_SHAPE_LT_NEG 3
//Solid area is the area above the line y=0.5-x
#define CLIP_SHAPE_GT_NEG 4
//Solid area is the area under the line y=abs(x-0.25) + 0.25
#define CLIP_SHAPE_GT_ABS 5
//Solid area is the area above the line y=-abs(x-0.25) + 0.25
#define CLIP_SHAPE_LT_ABS 6
//Solid area is the area above the line y=x-0.15 and under the line y=x+0.15
#define CLIP_SHAPE_IN_WALL_POS 7
//Solid area is the area above the line y=-x+0.35 and under the line y=-x+0.65
#define CLIP_SHAPE_IN_WALL_NEG 8

#define CLIP_SHAPE_COUNT 9

#define COL_DIR_FORWARD 1
#define COL_DIR_LEFT 2
#define COL_DIR_RIGHT 3

//The following arrays are lists of vertices of the voxel clip shapes
//Array of vertex count of the voxel clip shapes
extern int CLIP_SHAPE_VERT_COUNTS[CLIP_SHAPE_COUNT];

//These arrays hold the 2D vertices that make up the voxel clip shapes
extern float CLIP_SHAPE_VERTS_BOX[];
extern float CLIP_SHAPE_VERTS_LT_POS[];
extern float CLIP_SHAPE_VERTS_GT_POS[];
extern float CLIP_SHAPE_VERTS_LT_NEG[];
extern float CLIP_SHAPE_VERTS_GT_NEG[];
extern float CLIP_SHAPE_VERTS_GT_ABS[];
extern float CLIP_SHAPE_VERTS_LT_ABS[];
extern float CLIP_SHAPE_VERTS_IN_WALL_POS[];
extern float CLIP_SHAPE_VERTS_IN_WALL_NEG[];
//List of all shapes.
extern float* CLIP_SHAPE_SHAPES[CLIP_SHAPE_COUNT];

//Branching path def
#define BRANCH_TYPE_NONE 		0
#define BRANCH_TYPE_FORWARD 		1
#define BRANCH_TYPE_LEFT 		2
#define BRANCH_TYPE_RIGHT 		4
#define BRANCH_TYPE_FROM_FORWARD	8
#define BRANCH_TYPE_FROM_LEFT		32
#define BRANCH_TYPE_FROM_RIGHT	16


//Wall types: to be read as such:
//the wall always touches the center of the 3x3 tile
	//x is -x axis
	//X is +x axis
	//y is -y axis
	//Y is +y axis
	//o means the wall does not touch that axis
		//Example: xXoo		means the wall extends from -x to +x, through the center
		//Example: ooyY		means the wall extends from -y to +y, through the center
		//Example: xXyY		means the wall forms a '+' through the center
//Primitive wall types that make up the rest

#define WALL_TYPE_oooo 0//this wall type should be neither referenced nor accessed (it's an empty floor with no walls), it is here to maintain the binary assignment of walls
#define WALL_TYPE_xooo 1
#define WALL_TYPE_oXoo 2
#define WALL_TYPE_xXoo ( WALL_TYPE_xooo | 	WALL_TYPE_oXoo 								)//3
#define WALL_TYPE_ooyo 4
#define WALL_TYPE_xoyo ( WALL_TYPE_xooo | 					WALL_TYPE_ooyo 				)//5
#define WALL_TYPE_oXyo ( 				WALL_TYPE_oXoo | 	WALL_TYPE_ooyo 				)//6
#define WALL_TYPE_xXyo ( WALL_TYPE_xooo | 	WALL_TYPE_oXoo | 	WALL_TYPE_ooyo 				)//7
#define WALL_TYPE_oooY 8
#define WALL_TYPE_xooY ( WALL_TYPE_xooo | 									WALL_TYPE_oooY )//9
#define WALL_TYPE_oXoY ( 				WALL_TYPE_oXoo | 					WALL_TYPE_oooY )//10
#define WALL_TYPE_xXoY ( WALL_TYPE_xooo | 	WALL_TYPE_oXoo | 					WALL_TYPE_oooY )//11
#define WALL_TYPE_ooyY ( 							 	WALL_TYPE_ooyo | 	WALL_TYPE_oooY )//12
#define WALL_TYPE_xoyY ( WALL_TYPE_xooo | 					WALL_TYPE_ooyo | 	WALL_TYPE_oooY )//13
#define WALL_TYPE_oXyY ( 				WALL_TYPE_oXoo | 	WALL_TYPE_ooyo | 	WALL_TYPE_oooY )//14
#define WALL_TYPE_xXyY ( WALL_TYPE_xooo | 	WALL_TYPE_oXoo | 	WALL_TYPE_ooyo | 	WALL_TYPE_oooY )//15

#define WALL_TYPE_COUNT 16

//We define the above such that we can construct types out of other types. while maintaining the wall's spatial logic
// i.e. adding a left wall segment (xooo) and a right wall segment (oXoo) , yields a full horizontal wall (xXoo)
	// xooo | oXoo = xXoo


//Primitive rail types that make up the rest
#define RAIL_TYPE_NONE	0
#define RAIL_TYPE_L 	1
#define RAIL_TYPE_R 	2
#define RAIL_TYPE_TL 	4
#define RAIL_TYPE_TL2	8
#define RAIL_TYPE_TR 	16
#define RAIL_TYPE_TR2 	32

//Rail types that are composed of the above primitives
//						(RAIL_TYPE_L| 	RAIL_TYPE_R| 	RAIL_TYPE_TL|	RAIL_TYPE_TL2| 	RAIL_TYPE_TR| 	RAIL_TYPE_TR2	)
#define RAIL_TYPE_LR 		(RAIL_TYPE_L|	RAIL_TYPE_R														)//3
#define RAIL_TYPE_TL_TR 		(						RAIL_TYPE_TL|					RAIL_TYPE_TR				)//20
#define RAIL_TYPE_TL2_TR2	(									RAIL_TYPE_TL2 | 				RAIL_TYPE_TR2	)//40
#define RAIL_TYPE_TL_TL2		(						RAIL_TYPE_TL|	RAIL_TYPE_TL2								)//12
#define RAIL_TYPE_TR_TR2		(													RAIL_TYPE_TR| 	RAIL_TYPE_TR2	)//48
#define RAIL_TYPE_TL_L 		(RAIL_TYPE_L| 				RAIL_TYPE_TL											)//5
#define RAIL_TYPE_TL2_R 		(			RAIL_TYPE_R| 				RAIL_TYPE_TL2								)//10
#define RAIL_TYPE_TR_R 		(			RAIL_TYPE_R| 								RAIL_TYPE_TR				)//18
#define RAIL_TYPE_TR2_L 		(RAIL_TYPE_L| 														RAIL_TYPE_TR2	)//33

//Even though there are only 16 rail types, we want iteration and indexing to be possible with the above values (there will be holes in the arrays)
#define RAIL_TYPE_COUNT 64

//Because of these holes, we are going to hold an array to check whether a rail type is a valid rail type
extern bool RAIL_TYPE_IS_VALID[RAIL_TYPE_COUNT];


//Size of tiles (6 units x 6 units x 6 units)
#define TILE_SIZE 3.5f
#define WINDOW_TILE_SIZE 7.0f
#define GRID_SIZE 0.5f

#define LOWEST_FLOOR_ALLOWED 10

#define BUILDING_MAX_WIDTH 14
#define BUILDING_MIN_WIDTH 8

#define BUILDING_MAX_LENGTH 14
#define BUILDING_MIN_LENGTH 8

#define BUILDING_MAX_HEIGHT 40
#define BUILDING_MIN_HEIGHT 20
//Where the bottom of the buildings go in world space
#define BUILDING_GROUNDLEVEL -50
#define BUILDING_MAX_EXTERIOR_MODELS 50
#define BUILDING_MAX_INTERIOR_MODELS 10
#define BUILDING_WINDOW_MAX_TILE_MATRIX 5

//2 voxels per meter, 3.5m x 3.5m tile = 7v x 7v voxel map
#define TILE_VOXEL_DIMS 7
//Total number of voxels per tile (7x7)
#define TILE_VOXELS 49


//====================== Maneuver Keyframe Defs ==========================
//Lerp types
#define FRAME_LERP_LINEAR 0
#define FRAME_LERP_QUADRATIC 1
#define FRAME_LERP_QUAD_FROM_VERT 2
#define FRAME_LERP_QUAD_TO_VERT 3

//Player orientation types
#define FRAME_ORIENT_NONE 0
#define FRAME_ORIENT_ONCE 1
#define FRAME_ORIENT_CONSTANT 2

//Camera viewbob Identifiers
#define CAM_VIEWBOB_NONE 0
#define CAM_VIEWBOB_RUNNING 1
#define CAM_VIEWBOB_SLIDING 2

//Frame animation commands
#define FRAME_ANIM_NOOP 0
#define FRAME_ANIM_PLAY 1
#define FRAME_ANIM_PAUSE 2
#define FRAME_ANIM_RESUME 3
#define FRAME_ANIM_STOP 4
#define FRAME_ANIM_PLAY_DEFAULT 5

//speed types
//The player's speed is set as the value
#define FRAME_SPEED_CONST 0
//The player's speed is set as the value, but modified so that we reach the next keyframe in the same amount of time as
// if we were in the center of the bounds of this keyframe
#define FRAME_SPEED_CONST_TIME 1

//Special flags
#define FRAME_SPECFLAG_NONE 0
#define FRAME_SPECFLAG_BREAKWINDOW_OUT 1
#define FRAME_SPECFLAG_BREAKWINDOW_IN 4


//============================ Input Defs ==============================

//Defined as the percentage of the screen width the finger must be dragged before we register a swipe
#define INPUT_SENSITIVITY 0.0625f

//Input identifiers
#define INPUT_EVENT_ON_TOUCH_DOWN 1
#define INPUT_EVENT_ON_TOUCH_MOVE 2
#define INPUT_EVENT_ON_TOUCH_RELEASE 3

#define INPUT_SWIPE_NONE 1
#define INPUT_SWIPE_UP 2
#define INPUT_SWIPE_DOWN 4
#define INPUT_SWIPE_LEFT 8
#define INPUT_SWIPE_RIGHT 16

//Keyboard input defs
#define INPUT_KEY_BACK 1
#define INPUT_KEY_KEYBOARD 2

//Definition is in game_defs.cpp
//Array used for filtering out unwanted characters from keyboard input
//Here we filter out all control characters, except for ' ','\b', and '\n'. (keeps all symbolic & alphanumeric)
extern char INPUT_CHAR_FILTER[128];

//=========================== Game Logic Defs ==========================
#define MAX_BUILDINGS 3
extern int NEXT_BLDG[3];
extern int PREV_BLDG[3];

#define BUILDING_GAP 16

//Player States
#define PLAYER_STATE_MENU 0
#define PLAYER_STATE_RUNNING 1
#define PLAYER_STATE_FALLING 2
#define PLAYER_STATE_SLIDING 3
#define PLAYER_STATE_MANEUVERING 4
#define PLAYER_STATE_TRAVERSING 5

#define PLAYER_STATE_DEAD 6

#define PLAYER_STATE_NOCLIP 7
#define PLAYER_STATE_CAM_FLY 8

//Player Data
#define PLAYER_SIZE 0.3f
//For collision detection, if PLAYER_SIZE * 2 < GRID_SIZE, we can omit certain collision checks
//Basically, the player bbox will cover less voxels, requiring fewer checks.
//#define SMALL_PLAYER_BBOX


#define PLAYER_MAX_TURN_ANGLE 60.0f
#define PLAYER_RUN_SPEED 6.0f

#define PLAYER_TURN_LERP_FACTOR 0.5f

#define PLAYER_SLIDE_SPEED 6.0f
#define PLAYER_SLIDE_ACCEL -3.0f
#define PLAYER_SLIDE_MIN_SPEED 0.5f
//Slide time in seconds
#define PLAYER_SLIDE_TIME 1.1f


//#define PLAYER_JUMP_HEIGHT 0.75f
//jump vel = sqrt(PLAYER_JUMP_HEIGHT * 2.0f * 9.8f) (9.8f being acceleration of gravity)
#define PLAYER_JUMP_VEL 3.83406f

//Player Anim Defs
//NOTE: the order of these must match the order we load the animations in inside of Game.cpp
#define PLAYER_ANIM_RUN 0
#define PLAYER_ANIM_RUN_JUMP 1
#define PLAYER_ANIM_RUN_PRE_JUMP 2
#define PLAYER_ANIM_SLIDE 3
#define PLAYER_ANIM_SLIDE_END 4
#define PLAYER_ANIM_DIVE 5
#define PLAYER_ANIM_DIVE_END 6
#define PLAYER_ANIM_SPEED_VAULT 7
#define PLAYER_ANIM_VAULT_SLIDE 8
#define PLAYER_ANIM_KONG 9
#define PLAYER_ANIM_DASH_VAULT 10
#define PLAYER_ANIM_UNDERBAR 11
#define PLAYER_ANIM_HIGH_UNDERBAR 12

#define PLAYER_ANIM_TRAV_A 13
#define PLAYER_ANIM_TRAV_B 14
#define PLAYER_ANIM_TRAV_C 15

#define PLAYER_ANIM_DEATH_HITWALL 16
#define PLAYER_ANIM_DEATH_HITLEFT 17
#define PLAYER_ANIM_DEATH_HITRIGHT 18
#define PLAYER_ANIM_DEATH_HITWAIST 19
#define PLAYER_ANIM_DEATH_HITSLIDE 20
#define PLAYER_ANIM_DEATH_HITDOORWAY 21
#define PLAYER_ANIM_DEATH_HITWINDOW 22
#define PLAYER_ANIM_DEATH_SLIDEHITWALL 23
#define PLAYER_ANIM_DEATH_SLIDEHITLEFT 24
#define PLAYER_ANIM_DEATH_SLIDEHITRIGHT 25
#define PLAYER_ANIM_DEATH_SLIDEHITWINDOW 26
#define PLAYER_ANIM_SHOWCASE_HANDS 27

//These are defs for the behavior of the game when it's game over
//How long it takes for the black screen to fade in upon death
#define DEATH_TIME_BLACK_SCREEN_FADEIN 0.1f
#define DEATH_TIME_BLACK_SCREEN_FALLING_FADEIN 0.5f
//How long we spend at the black screen
#define DEATH_TIME_BLACK_SCREEN 0.7f
//How long it takes for the white screen to fade in
#define DEATH_TIME_WHITE_SCREEN_FADEIN 0.3f
//How long we spend at the white screen
#define DEATH_TIME_WHITE_SCREEN 0.0f
//How long it takes for the white screen to fade out
#define DEATH_TIME_WHITE_SCREEN_FADEOUT 0.4f

//Window Breaking Anim Defs
//out and in are the same, since they are the both the first animations of their skeletons
#define WINDOW_ANIM_BREAK 0


#define BUILDING_TRAV_COUNT 3



#define PARKOUR_GAME_DEFS_HPP

#endif //PARKOUR_GAME_DEFS_HPP
