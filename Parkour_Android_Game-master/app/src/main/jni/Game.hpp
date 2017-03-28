//
// Created by F1 on 8/2/2016.
//

#ifndef ENGINE_GAME_HPP
#define ENGINE_GAME_HPP

#include "engine/common.hpp"
#include "engine/graphics/Camera.hpp"
#include "entities/Player.hpp"
#include "engine/graphics/UI.hpp"
#include "engine/graphics/Skybox.hpp"
#include "engine/sound/Audio_Engine.hpp"
#include "engine/JNI_Interface.hpp"
#include "entities/Building.hpp"
#include "game/Global_Tiles.hpp"
#include "game/Dynamic_Model.hpp"
#include "entities/Player_Head.hpp"


class Game
{
public:

	Game()
	{
	}

	~Game()
	{
	}

	//Variables that engine uses
	Camera *audio_listener;
	int32_t screen_width;
	int32_t screen_height;
	float screen_ratio;// width/height

	//Access to engine-initiated JNI interface
	JNI_Interface *jnii;

	//=========================== Game Asset Variables ====================
	//-------------- Sound Files --------------
	Sound_Sample *test_pulse = NULL;
	Sound_Sample *snd_office_amb = NULL;
	Sound_Sample *snd_highrise_amb = NULL;
	Sound_Sample *snd_winbreak_out = NULL;
	Sound_Sample *snd_winbreak_in = NULL;
	Sound_Sample *snd_jumpwind = NULL;
	Sound_Sample *snd_death_impact = NULL;
	Sound_Sample *snd_death_trans = NULL;
	Sound_Sample *snd_breath = NULL;
	Sound_Sample *snd_breath_jump = NULL;
	Sound_Sample *snd_breath_land = NULL;

	Sound_Sample *footstep_sounds[8];

	Sound_Sample *snd_hand_contact_low = NULL;
	Sound_Sample *snd_hand_contact_mid = NULL;
	Sound_Sample *snd_hand_contact_high = NULL;
	Sound_Sample *snd_slide = NULL;

	Sound_Sample *snd_roll = NULL;
	Sound_Sample *snd_hip_contact = NULL;
	Sound_Sample *snd_land_contact = NULL;
	Sound_Sample *snd_kick_elevator = NULL;
	Sound_Sample *snd_hand_metal_contact = NULL;
	Sound_Sample *snd_body_hit_slide = NULL;


	Sound_Source *player_breath_src = NULL;

	Sound_Source *office_ambience_src = NULL;
	Sound_Source *highrise_ambience_src = NULL;

	//-------------- Shaders ------------------
	Shader *test_shader = NULL;
	Shader *skel_color_shader = NULL;
	Shader *static_color_shader = NULL;
	Shader *text_shader = NULL;
	Shader *player_skin_shader = NULL;
	Shader *solid_shader = NULL;
	Shader *screen_overlay_shader = NULL;

	//------------- Materials -----------------
	Material *mat_red = NULL;
	Material *mat_blue = NULL;
	Material *skel_color_mat= NULL;
	Material *static_color_mat = NULL;
	Material *text_mat = NULL;
	Material *player_skin_mat = NULL;
	Material *player_torso_mat = NULL;
	Material *player_leg_mat = NULL;
	Material *solid_mat = NULL;
	Material *screen_overlay_mat = NULL;

	//------------ Textures -------------------
	Texture *test_texture = NULL;
	Texture *char_set = NULL;
	Texture *tex_arm_nor = NULL;
	Texture *tex_arm_diff = NULL;
	Texture *tex_torso_nor = NULL;
	Texture *tex_torso_diff = NULL;
	Texture *tex_leg_nor = NULL;
	Texture *tex_leg_diff = NULL;

	//------------- Models --------------------
	Skel_Model *test_arms = NULL;
	Skel_Model *test_torso = NULL;
	Skel_Model *test_legs = NULL;

	Static_Model *model_prim_cube = NULL;
	Static_Model *model_prim_quad = NULL;

	Static_Model *test_model_int_empty = NULL;

	//---------- Skeletons --------------------
	Skeleton *player_skel = NULL;
	Skeleton_Data *player_skel_data = NULL;



	//=========================== Game Object Variables ====================
	Camera *camera = NULL;
	Player *player = NULL;
	Player_Head *player_head = NULL;
	Entity_Bone_Joint *cam_to_bone = NULL;

	Skybox *skybox = NULL;

	Building **buildings;
	Building *current_building = NULL;
	int cbldg_index = 0;

	int last_recycled_bldg_index = MAX_BUILDINGS - 1;
	int bldgs_jumped = 0;
	bool recycle_every_time = false;

	Dynamic_Model *dynamic_floor_models[MAX_BUILDINGS];

	//======================================================================


	//========================================= Loading assets ========================player==============

	int load_shaders();
	void unload_shaders();

	int load_materials();
	void unload_materials();

	int load_textures();
	void unload_textures();

	int load_models();
	void unload_models();

	int load_sounds();
	void unload_sounds();

	int load_assets();
	void unload_assets();
	//=================================================================================================
	//Initialize GL aspects of all assets
	int init_gl();

	void term_gl();

	//Ran on first frame
	//This is where we set up our game objects and their relationships
	void start();
	//Ran on last frame
	//This is where we destroy our game objects
	void finish();

	//input variables
	#define MAX_INPUT_TOUCHES 10
	bool input_touching[10] = {false,false,false,false,false,false,false,false,false,false};
	float input_x[10] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	float input_y[10] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	float input_start_x[10] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};
	float input_start_y[10] = {0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f,0.0f};

	bool input_turning = false;
	bool input_sent_command = false;

	int input_swipe;

	float input_turn;

	//Handles touch screen input
	void handle_input(float x, float y, int event,int pointer_id);
	//Handles hardware back key / keyboard input
	void handle_key_input(int event_type, char event_key_char);

	Vec3 player_phys_vel;

	int player_state = 0;
	float player_substate = 0.0f;
	//Misc variable used for timing something the state needs to time
	float player_substate_time = 0.0f;
	//Second misc variable used for timing something else that the state may need
	float player_substate_time2 = 0.0f;

	//Sliding variables
	float player_slide_speed = 0.0f;


	//FIXME: remove this
	//===================
	//int viewbob_menu_state = 0;
	float viewbob_pitch = 11.5f;
	float viewbob_yaw = 14.5f;
	float viewbob_roll = 11.5f;

	bool player_colliding = false;
	//===================

	float player_bbox_tri_slope = 0.0f;
	float player_bbox_tri_height = 0.0f;

	//If RENDER_AT_LOWER_RES is defined in engine_defs, engine uses the following float for which resolution to render the game at
	float engine_render_resolution_scale = 0.75f;

	//Updates the game state / logic
	void update();

	//Draws the scene
	void render();

	//==== Game logic specific methods =====
	//Handles player collision detection and player movement
	bool move_player(Vec3 v);
	//Handles player bbox hull collision with building->floor->tile voxels
	//Returns whether or not the collision player bbox is colliding with something at position p
	//Sets col_dir and col_type to
	bool clip_player_bbox(Vec3 p, char& col_dir, char& col_type);

	//Plays a player death animation
	//which animation depends on collision direction and collision type
	void start_player_death(char dir, char type);

	//Returns true if all points (2d coords) in pts are on the left side of line l1->l2
	//(left is defined from standing at l1, looking at l2)
	bool clip_all_on_left_side(Vec3 l1, Vec3 l2,float *pts, int pt_count);
	//Given portion of bbox outlined by (la,lb), check if vox is outside of the bbox line
	char voxel_not_in_line(Vec3 la, Vec3 lb, Voxel vox);
	//Given portion of bbox outlined by both (l1a,l1b) and (l2a,l2b), , check if vox is outside of both bbox lines
	char voxel_not_in_lines(Vec3 l1a, Vec3 l1b, Vec3 l2a, Vec3 l2b, Voxel vox);

	//Returns if the pos's x coordinate is within the area required to start a building to building traversal
	//This only checks the x-coordinate
	bool is_in_traversal_x_bounds(Vec3 pos);


	//Executes code if player is at specific frames in specific animations
	void player_anim_special_events();

	//Holds the player state logic
	void player_state_logic();

	//Holds the player noclip logic (touch input and noclip flying about)
	void player_noclip_logic();

	//Draws the screen overlay (a slight edge darkening on the screen)
	//This also handles the black / white overlays on the screen
	void render_screen_overlay();

	//These are the variables that control the screen overlay effects
	float edge_darken_opacity = 0.3f;
	float black_overlay_opacity = 0.0f;
	float white_overlay_opacity = 0.0f;


	//The following 3 arrays hold the priority of each of the following clip types, in their respective indices
	// (Higher value = higher priority)
	//{CLIP_EMPTY,CLIP_SOLID,CLIP_SLIDE,CLIP_DOORWAY,CLIP_MID}
	char running_col_precedence[5] = {0,4,3,0,2};
	char jumping_col_precedence[5] = {0,4,3,2,1};
	char sliding_col_precedence[5] = {0,4,2,0,3};
	char* cplayer_col_precedence_array = NULL;

	//Draws player bounding box
	void draw_player_bbox(Mat4 vp);
	//Draws active floors collision voxels
	void draw_floor_collision_voxels(Mat4 vp);
	//Draws active floors maneuver data
	void draw_floor_maneuvers(Mat4 vp);
	//Draws active floor maneuver keyframes
	void draw_keyframe(Mat4 vp, Keyframe* key, Vec3 ofs);

	//====== Maneuver variables ======
	Maneuver* mnvr_current;
	Traversal* trav_current;
	Keyframe* mnvr_frame;
	int mnvr_frame_number;
	Keyframe* mnvr_next_frame;
	int mnvr_next_frame_number;
	//Global position of the tile this maneuver is owned by
	Vec3 mnvr_tile_ofs;
	//Global goal position we're moving to
	Vec3 mnvr_goal_pos;
	//Global start position we're moving to
	Vec3 mnvr_start_pos;
	//Yaw Rotation Orientation of player
	float mnvr_goal_yaw_rot;
	//current y-axis speed of keyframe
	float mnvr_y_vel;
	//Returns the pos if in mins & maxs, otherwise caps to lie within the mins & maxs
	Vec3 cap_to_bounds(Vec3 pos, Vec3 mins, Vec3 maxs);
	//Variables used for quadratic movement
	float mnvr_var_a;
	float mnvr_var_b;
	float mnvr_var_c;

	//Slope of the line that makes up the player path in the x-y axis
	float mnvr_var_x_slope;

	void mnvr_movement();
	//Sets up the movement from the frame we're at, to the next frame in the sequence (or last frame)
	void reached_mnvr_keyframe();
	//Returns angle from player pos to the orientation position of the keyframe
	float get_keyframe_goal_yaw(Keyframe* key);

	//Roll of camera that changes when we turn rapidly
	float camera_roll_tilt_angle = 0.0f;
	float player_goal_yaw = 0.0f;
	bool lock_player_rot = false;

	//Prepares the game to run again
	void reset();

	//Contains the code that makes the player run
	void player_run();

};

#endif //ENGINE_GAME_HPP
