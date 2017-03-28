//
// Created by F1 on 8/2/2016.
//

#include "Game.hpp" #include "game/Maneuver.hpp"

int Game::load_shaders ()
{
	//Initializing shaders

	//Test shader
	GLuint pt1[] =
	{
		Shader::PARAM_VERTICES,
		Shader::PARAM_VERT_COLORS,
		Shader::PARAM_VERT_UV1,
		Shader::PARAM_TEXTURE_DIFFUSE,
		Shader::PARAM_MVP_MATRIX,
		Shader::PARAM_TEST_FIELD
	};
	const char *pn1[] =
	{
		"vert_pos",
		"fill_color",
		"src_tex_coord",
		"tex",
		"mvp",
		"test_color_param"
	};
	test_shader = new Shader("shaders/minimal.vert","shaders/minimal.frag",pt1,pn1,6);

	//Text shader
	GLuint pt2[] =
	{
		Shader::PARAM_VERTICES,
		Shader::PARAM_VERT_UV1,
		Shader::PARAM_TEXTURE_DIFFUSE,
		Shader::PARAM_MVP_MATRIX,
		Shader::PARAM_COLOR_MULT,
		Shader::PARAM_COLOR_ADD
	};
	const char *pn2[] =
	{
		"vert_pos",
		"src_tex_coord",
		"tex",
		"mvp",
		"mult_color",
		"add_color"
	};
	text_shader = new Shader("shaders/monochrome_transparent.vert","shaders/monochrome_transparent.frag",pt2,pn2,6);

	//Skeleton test shader
	GLuint pt3[] =
	{
		Shader::PARAM_VERTICES,
		Shader::PARAM_VERT_UV1,
		Shader::PARAM_VERT_NORMALS,
		Shader::PARAM_MVP_MATRIX,
		Shader::PARAM_M_IT_MATRIX,
		Shader::PARAM_BONE_INDICES,
		Shader::PARAM_BONE_WEIGHTS,
		Shader::PARAM_BONE_MATRICES,
		Shader::PARAM_BONE_IT_MATRICES
	};
	const char *pn3[] =
	{
		"vert_pos",
		"vert_uv",
		"vert_nor",
		"mvp",
		"m_IT",
		"bone_index",
		"bone_weight",
		"bone",
		"bone_IT"
	};
	skel_color_shader = new Shader("shaders/skeletal_color.vert","shaders/skeletal_color.frag",pt3,pn3,9);

	//Player skin shader
	GLuint pt4[] =
	{
		Shader::PARAM_VERTICES,
		Shader::PARAM_VERT_UV1,
		Shader::PARAM_VERT_NORMALS,
		Shader::PARAM_VERT_TANGENTS,
		Shader::PARAM_VERT_BINORMALS,
		Shader::PARAM_TEXTURE_NORMAL,
		Shader::PARAM_TEXTURE_DIFFUSE,
		Shader::PARAM_MVP_MATRIX,
		Shader::PARAM_M_IT_MATRIX,
		Shader::PARAM_BONE_INDICES,
		Shader::PARAM_BONE_WEIGHTS,
		Shader::PARAM_BONE_MATRICES,
		Shader::PARAM_BONE_IT_MATRICES
	};
	const char *pn4[] =
	{
		"vert_pos",
		"vert_uv",
		"vert_nor",
		"vert_tan",
		"vert_binor",
		"tex_nor",
		"tex_diff",
		"mvp",
		"m_IT",
		"bone_index",
		"bone_weight",
		"bone",
		"bone_IT"
	};
	player_skin_shader = new Shader("shaders/player_skin.vert","shaders/player_skin.frag",pt4,pn4,13);

	//Test static model shader
	GLuint pt5[] =
	{
		Shader::PARAM_VERTICES,
		Shader::PARAM_VERT_UV1,
		Shader::PARAM_VERT_UV2,
		Shader::PARAM_VERT_NORMALS,
		Shader::PARAM_MVP_MATRIX,
		Shader::PARAM_M_IT_MATRIX,
		Shader::PARAM_COLOR_MULT
	};
	const char *pn5[] =
	{
		"vert_pos",
		"vert_uv_1",
		"vert_uv_2",
		"vert_nor",
		"mvp",
		"m_IT",
		"color"
	};
	static_color_shader = new Shader("shaders/static_color.vert","shaders/static_color.frag",pt5,pn5,7);


	//Solid shader (draws as fullbright single color)
	//Test shader
	GLuint pt6[] =
	{
		Shader::PARAM_VERTICES,
		Shader::PARAM_MVP_MATRIX,
		Shader::PARAM_COLOR_ADD
	};
	const char *pn6[] =
	{
		"vert_pos",
		"mvp",
		"color"
	};
	solid_shader= new Shader("shaders/solid_color.vert","shaders/solid_color.frag",pt6,pn6,3);

	//Screen Overlay shader (drawn above the 3d world)
	//Test shader
	GLuint pt7[] =
	{
	Shader::PARAM_VERTICES,
	Shader::PARAM_COLOR_MULT
	};
	const char *pn7[] =
	{
	"vert_pos",
	"info"
	};
	screen_overlay_shader = new Shader("shaders/screen_overlay.vert","shaders/screen_overlay.frag",pt7,pn7,2);


	return 1;
}
void Game::unload_shaders ()
{
	delete test_shader;
	delete skel_color_shader;
	delete static_color_shader;
	delete text_shader;
	delete player_skin_shader;
	delete solid_shader;
	delete screen_overlay_shader;
}

int Game::load_materials()
{
	mat_red = new Material();
	mat_blue = new Material();
	skel_color_mat = new Material();
	static_color_mat = new Material();
	text_mat = new Material();
	player_skin_mat = new Material();
	player_torso_mat = new Material();
	player_leg_mat = new Material();
	solid_mat = new Material();
	screen_overlay_mat = new Material();

	return 1;
}
void Game::unload_materials()
{
	delete mat_red;
	delete mat_blue;
	delete skel_color_mat;
	delete text_mat;
	delete player_skin_mat;
	delete player_torso_mat;
	delete player_leg_mat;
	delete solid_mat;
	delete screen_overlay_mat;
}

int Game::load_textures()
{
	test_texture = new Texture("tex.pkm",512,512);
	char_set = new Texture("char_set.pkm",2048,2048);
	tex_arm_nor = new Texture("textures/arm_nor.pkm",1024,1024);
	tex_arm_diff = new Texture("textures/arm_diff.pkm",1024,1024);
	tex_torso_nor = new Texture("textures/torso_nor.pkm",512,512);
	tex_torso_diff = new Texture("textures/torso_diff.pkm",512,512);
	tex_leg_nor = new Texture("textures/leg_nor.pkm",512,512);
	tex_leg_diff = new Texture("textures/leg_diff.pkm",512,512);
	return 1;
}
void Game::unload_textures()
{
	delete test_texture;
	delete char_set;
	delete tex_arm_nor;
	delete tex_arm_diff;
	delete tex_torso_nor;
	delete tex_torso_diff;
	delete tex_leg_nor;
	delete tex_leg_diff;
}

int Game::load_models()
{
	for(int i = 0; i < MAX_BUILDINGS; i++)
	{
		dynamic_floor_models[i] = new Dynamic_Model();
	}

	test_arms = new Skel_Model("models/test_arms.skmf");
	test_torso = new Skel_Model("models/test_torso.skmf");
	test_legs = new Skel_Model("models/test_legs.skmf");

	model_prim_cube = new Static_Model("models/primitive_cube.stmf");
	model_prim_quad = new Static_Model("models/primitive_quad.stmf");

	test_model_int_empty = new Static_Model("models/tiles/style0/empt0.stmf");

	player_skel_data = new Skeleton_Data("animations/player_skeleton.sksf");

	//The order of loading these animations must match the animation identifiers
	player_skel_data->load_animation("animations/run.skaf");
	player_skel_data->load_animation("animations/run_jump.skaf");
	player_skel_data->load_animation("animations/run_prejump.skaf");
	player_skel_data->load_animation("animations/slide.skaf");
	player_skel_data->load_animation("animations/slide_end.skaf");
	player_skel_data->load_animation("animations/dive.skaf");
	player_skel_data->load_animation("animations/dive_end.skaf");
	player_skel_data->load_animation("animations/speed_vault.skaf");
	player_skel_data->load_animation("animations/vault_slide.skaf");
	player_skel_data->load_animation("animations/kong.skaf");
	player_skel_data->load_animation("animations/dash_vault.skaf");
	player_skel_data->load_animation("animations/underbar.skaf");
	player_skel_data->load_animation("animations/high_underbar.skaf");
	player_skel_data->load_animation("animations/traversal_a.skaf");
	player_skel_data->load_animation("animations/traversal_b.skaf");
	player_skel_data->load_animation("animations/traversal_c.skaf");
	player_skel_data->load_animation("animations/death_hitwall.skaf");
	player_skel_data->load_animation("animations/death_hitleft.skaf");
	player_skel_data->load_animation("animations/death_hitright.skaf");
	player_skel_data->load_animation("animations/death_hitwaist.skaf");
	player_skel_data->load_animation("animations/death_hitslide.skaf");
	player_skel_data->load_animation("animations/death_hitdoorway.skaf");
	player_skel_data->load_animation("animations/death_hitwindow.skaf");
	player_skel_data->load_animation("animations/death_slidehitwall.skaf");
	player_skel_data->load_animation("animations/death_slidehitleft.skaf");
	player_skel_data->load_animation("animations/death_slidehitright.skaf");
	player_skel_data->load_animation("animations/death_slidehitwindow.skaf");
	player_skel_data->load_animation("animations/showcase_hands.skaf");
	//NOTE: any animation added here must also be added as an identifier in game_defs.hpp

	skybox = new Skybox();
	return 1;
}
void Game::unload_models()
{
	for(int i = 0; i < MAX_BUILDINGS; i++)
	{
		delete dynamic_floor_models[i];
	}
	delete test_arms;
	delete test_torso;
	delete test_legs;

	delete model_prim_cube;
	delete model_prim_quad;

	delete test_model_int_empty;
	delete player_skel;
	delete player_skel_data;

	delete skybox;
}

int Game::load_sounds()
{
	test_pulse = new Sound_Sample("sounds/test_audio_pulse.raw");
	snd_office_amb = new Sound_Sample("sounds/office_amb.raw");
	snd_highrise_amb = new Sound_Sample("sounds/highrise_amb.raw");
	snd_winbreak_in = new Sound_Sample("sounds/break_glass_in.raw");
	snd_winbreak_out = new Sound_Sample("sounds/break_glass_out.raw");
	snd_jumpwind = new Sound_Sample("sounds/jump_wind.raw");
	snd_death_impact = new Sound_Sample("sounds/death_impact.raw");
	snd_death_trans = new Sound_Sample("sounds/death_trans.raw");

	footstep_sounds[0] = new Sound_Sample("sounds/footstep_0.raw");
	footstep_sounds[1] = new Sound_Sample("sounds/footstep_1.raw");
	footstep_sounds[2] = new Sound_Sample("sounds/footstep_2.raw");
	footstep_sounds[3] = new Sound_Sample("sounds/footstep_3.raw");
	footstep_sounds[4] = new Sound_Sample("sounds/footstep_4.raw");
	footstep_sounds[5] = new Sound_Sample("sounds/footstep_5.raw");
	footstep_sounds[6] = new Sound_Sample("sounds/footstep_6.raw");
	footstep_sounds[7] = new Sound_Sample("sounds/footstep_7.raw");

	snd_breath = new Sound_Sample("sounds/breath.raw");
	snd_breath_jump = new Sound_Sample("sounds/breath_jump.raw");
	snd_breath_land = new Sound_Sample("sounds/breath_land.raw");

	snd_hand_contact_low = new Sound_Sample("sounds/hand_contact_soft.raw");
	snd_hand_contact_mid = new Sound_Sample("sounds/hand_contact_mid.raw");
	snd_hand_contact_high = new Sound_Sample("sounds/hand_contact_hard.raw");
	snd_slide = new Sound_Sample("sounds/slide.raw");

	snd_roll = new Sound_Sample("sounds/roll.raw");
	snd_hip_contact = new Sound_Sample("sounds/hip_contact.raw");
	snd_land_contact = new Sound_Sample("sounds/land_contact.raw");
	snd_kick_elevator = new Sound_Sample("sounds/kick_elevator.raw");
	snd_hand_metal_contact = new Sound_Sample("sounds/hand_metal_contact.raw");
	snd_body_hit_slide = new Sound_Sample("sounds/hit_slide.raw");

	return 1;
}
void Game::unload_sounds()
{
	delete test_pulse;
	for(int i = 0; i < 8; i++)
	{
		delete footstep_sounds[i];
	}

	delete snd_breath;
	delete snd_breath_jump;
	delete snd_breath_land;
	delete snd_office_amb;
	delete snd_highrise_amb;
	delete snd_winbreak_in;
	delete snd_winbreak_out;
	delete snd_jumpwind;
	delete snd_death_impact;
	delete snd_death_trans;

	delete snd_hand_contact_low;
	delete snd_hand_contact_mid;
	delete snd_hand_contact_high;
	delete snd_slide;

	delete snd_roll;
	delete snd_hip_contact;
	delete snd_land_contact;
	delete snd_kick_elevator;
	delete snd_hand_metal_contact;
	delete snd_body_hit_slide;
}

int Game::load_assets()
{
	if(!Global_Tiles::init_data())
		return 0;
	if(!load_textures())
		return 0;
	if(!load_shaders())
		return 0;
	if(!load_materials())
		return 0;
	if(!load_models())
		return 0;
	if(!load_sounds())
		return 0;
	return 1;
}
void Game::unload_assets()
{
	unload_sounds();
	unload_models();
	unload_materials();
	unload_shaders();
	unload_textures();
	Global_Tiles::term_data();
}
//=================================================================================================
//Initialize GL aspects of all assets
int Game::init_gl()
{
	//=================================== Initializing shaders ====================================
	test_shader->init_gl();
	text_shader->init_gl();
	skel_color_shader->init_gl();
	player_skin_shader->init_gl();
	static_color_shader->init_gl();
	solid_shader->init_gl();
	screen_overlay_shader->init_gl();

	//==================================== Loading textures =======================================
	test_texture->init_gl();
	char_set->init_gl();
	tex_arm_nor->init_gl();
	tex_arm_diff->init_gl();
	tex_torso_nor->init_gl();
	tex_torso_diff->init_gl();
	tex_leg_nor->init_gl();
	tex_leg_diff->init_gl();
	//==================================== Setting up Mesh VBOs ====================================
	test_arms->init_gl();
	test_torso->init_gl();
	test_legs->init_gl();
	model_prim_cube->init_gl();
	model_prim_quad->init_gl();
	for(int i = 0; i < MAX_BUILDINGS; i++)
	{
		dynamic_floor_models[i]->init_gl();
	}

	test_model_int_empty->init_gl();

	skybox->init_gl();

	Global_Tiles::init_gl();

	glLineWidth(4.0f);

	return 1;
}

void Game::term_gl()
{
	//Terminating all loaded shaders
	player_skin_shader->term_gl();
	test_shader->term_gl();
	skel_color_shader->term_gl();
	static_color_shader->term_gl();
	text_shader->term_gl();
	solid_shader->term_gl();
	screen_overlay_shader->term_gl();

	//Terminating all loaded models
	test_arms->term_gl();
	test_torso->term_gl();
	test_legs->term_gl();
	model_prim_cube->term_gl();
	model_prim_quad->term_gl();
	for(int i = 0; i < MAX_BUILDINGS; i++)
	{
		dynamic_floor_models[i]->term_gl();
	}

	test_model_int_empty->term_gl();

	skybox->term_gl();

	Global_Tiles::term_gl();

	//Terminating all loaded textures
	tex_arm_nor->term_gl();
	tex_arm_diff->term_gl();
	tex_torso_diff->term_gl();
	tex_leg_diff->term_gl();
	char_set->term_gl();
	test_texture->term_gl();
}

void Game::handle_input(float x, float y, int event, int pointer_id)
{
	y = 1.0f - y;

	//Only do general stuff for other pointers, not 0th one
	//	(more complex input handling for 0th pointer is done below)
	//		(the handling for the 0th pointer also includes this general stuff)
	if(pointer_id != 0)
	{
		//General input handling stuff:
		if(event == INPUT_EVENT_ON_TOUCH_DOWN)
		{
			input_touching[pointer_id] = true;
			input_x[pointer_id] = x;
			input_y[pointer_id] = y;
			input_start_x[pointer_id] = x;
			input_start_y[pointer_id] = y;
		}
		if(event == INPUT_EVENT_ON_TOUCH_MOVE)
		{
			input_x[pointer_id] = x;
			input_y[pointer_id] = y;
		}
		if(event == INPUT_EVENT_ON_TOUCH_RELEASE)
		{
			input_x[pointer_id] = 0.0f;
			input_y[pointer_id] = 0.0f;
			input_start_x[pointer_id] = 0.0f;
			input_start_y[pointer_id] = 0.0f;
			input_touching[pointer_id] = false;
		}
		return;
	}

	//TODO: handle gamestates such as menu / pause menu

	if(event == INPUT_EVENT_ON_TOUCH_DOWN)
	{
		input_turning = false;
		input_sent_command = false;
		input_touching[0] = true;
		input_start_x[0] = x;
		input_start_y[0] = y;
	}
	else if(event == INPUT_EVENT_ON_TOUCH_RELEASE)
	{
		input_turning = false;
		input_sent_command = false;
		input_touching[0] = false;
		input_x[0] = 0.0f;
		input_y[0] = 0.0f;
		input_start_x[0] = 0.0f;
		input_start_y[0] = 0.0f;
		return;
	}

	switch(event)
	{
		case INPUT_EVENT_ON_TOUCH_DOWN:
		case INPUT_EVENT_ON_TOUCH_MOVE:
		case INPUT_EVENT_ON_TOUCH_RELEASE:
			input_x[0] = x;
			input_y[0] = y;
			break;
		default:
			break;
	}

	input_turn = 0;
	input_swipe = 0;

	if(input_sent_command)
		return;

	float delta_x = input_x[0] - input_start_x[0];
	float delta_y = input_y[0] - input_start_y[0];
	float delta_x_abs = fabsf(delta_x);
	float delta_y_abs = fabsf(delta_y);

	//Check if we are swiping horizontally or already turning
	if(input_turning || ((delta_x_abs > delta_y_abs) && (delta_x_abs > INPUT_SENSITIVITY)))
	{
		//if input_sensitivity is 1/16, the 16.0f and it annihilate each other
		//Making us only have to swipe 1/3rd of the screen width to go through the full range of motion
		input_turn = 3.0f * 16.0f * delta_x * INPUT_SENSITIVITY;
		input_turn = fmaxf(-1.0f,fminf(1.0f,input_turn));

		input_turning = true;
		return;
	}

	//Check if we are swiping vertically
	if(delta_y_abs > delta_x_abs)
	{
		if(delta_y > INPUT_SENSITIVITY*screen_ratio)
		{
			input_swipe = INPUT_SWIPE_UP;
			input_sent_command = true;
		}
		if(delta_y < - INPUT_SENSITIVITY*screen_ratio)
		{
			input_swipe = INPUT_SWIPE_DOWN;
			input_sent_command = true;
		}
		return;
	}
}

//Handles hardware back key / keyboard input
void Game::handle_key_input(int event_type, char event_key_char)
{
	if(event_type == INPUT_KEY_BACK)
	{
		//TODO: handle back key logic
		//TODO: handle the following situation:
		//TODO: 	if the keyboard is visible and back key is pressed, the keyboard is hidden
		LOGI("The back key has been pressed");
	}
	if(event_type == INPUT_KEY_KEYBOARD)
	{
		LOGI("Keyboard key pressed: (ascii value: %d) char: %c",event_key_char,event_key_char);
	}
}

//Ran on first frame
//This is where we set up our game objects and their relationships
void Game::start()
{
	camera = new Camera();
	player = new Player();
	player_head = new Player_Head();
	cam_to_bone = new Entity_Bone_Joint();

	//===== Setting up relationships between game objects ======
	audio_listener = camera;

	//Setting run anim as default anim
	player->mat1 = player_skin_mat;
	player->mat2 = player_torso_mat;
	player->mat3 = player_leg_mat;

	player_skel = new Skeleton(player_skel_data);
	player_skel->set_default_anim(0,ANIM_END_TYPE_LOOP);


	//============================= Setting up materials ================================
	mat_red->set_shader(test_shader);
	mat_blue->set_shader(test_shader);
	text_mat->set_shader(text_shader);
	skel_color_mat->set_shader(skel_color_shader);
	static_color_mat->set_shader(static_color_shader);
	player_skin_mat->set_shader(player_skin_shader);
	player_torso_mat->set_shader(player_skin_shader);
	player_leg_mat->set_shader(player_skin_shader);
	solid_mat->set_shader(solid_shader);
	screen_overlay_mat->set_shader(screen_overlay_shader);

	player_skin_mat->set_fixed_shader_param_ptr(Shader::PARAM_TEXTURE_NORMAL,(void*) tex_arm_nor);
	player_skin_mat->set_fixed_shader_param_ptr(Shader::PARAM_TEXTURE_DIFFUSE,(void*) tex_arm_diff);

	player_torso_mat->set_fixed_shader_param_ptr(Shader::PARAM_TEXTURE_NORMAL,(void*) tex_torso_nor);
	player_torso_mat->set_fixed_shader_param_ptr(Shader::PARAM_TEXTURE_DIFFUSE,(void*) tex_torso_diff);

	player_leg_mat->set_fixed_shader_param_ptr(Shader::PARAM_TEXTURE_NORMAL,(void*) tex_leg_nor);
	player_leg_mat->set_fixed_shader_param_ptr(Shader::PARAM_TEXTURE_DIFFUSE,(void*) tex_leg_diff);

	//Setting up fixed shader parameters
	float temp_color[] = {1.0f, 1.0f, 0.0f, 1.0f};
	static_color_mat->set_fixed_shader_param(Shader::PARAM_COLOR_MULT,temp_color,sizeof(float)*4);
	float temp_color_red[] = {1.0f, 0.4f, 0.4f, 1.0f};
	mat_red->set_fixed_shader_param(Shader::PARAM_TEST_FIELD, temp_color_red, sizeof(float) * 4);
	mat_red->set_fixed_shader_param_ptr(Shader::PARAM_TEXTURE_DIFFUSE, (void*) test_texture);
	float temp_color_blue[] = {0.4f, 0.4f, 1.0f, 1.0f};
	mat_blue->set_fixed_shader_param(Shader::PARAM_TEST_FIELD, temp_color_blue, sizeof(float) * 4);

	//===================================================================================

	skybox->set_cube_map(Global_Tiles::instance->sky_cube_map);


	//===== Instantiating Game Objects =====
	//Setting default text mat/shader/charset tex
	UI_Text::set_default_values(text_mat,char_set);

	player->player_model1 = test_arms;
	player->player_model2 = test_torso;
	player->player_model3 = test_legs;
	player->skel = player_skel;

	camera->parent = player_head;
	player_head->parent = cam_to_bone;
	cam_to_bone->parent_skel = player_skel;
	cam_to_bone->parent_bone_index = 8; //head bone is at index 8, we could add methods for finding the bone
	// but we don't need all of that at the moment (since we are never going to parent anything to any other bone)
	player_skel->parent = player;
	player_skel->pos.z = 1.0f;
	camera->set_persp_view(90.0f * DEG_TO_RAD, screen_width,screen_height, 0.01f, 1000.0f);
	camera->set_ortho_view(screen_width,screen_height,0.0001f,1.0f);

	//Evaluating player bounding box triangle constants
	player_bbox_tri_slope = tanf((90.0f - PLAYER_MAX_TURN_ANGLE) * DEG_TO_RAD);
	player_bbox_tri_height = player_bbox_tri_slope * PLAYER_SIZE;


	//Start of game logic code
	buildings = new Building*[MAX_BUILDINGS];

	for(int i = 0; i < MAX_BUILDINGS; i++)
	{
		buildings[i] = new Building();
		buildings[i]->active_floor->dynamic_floor_model = dynamic_floor_models[i];
		buildings[i]->active_floor->debug_branch_mat = solid_mat;

		buildings[i]->broken_iwindow_skel = new Skeleton(Global_Tiles::instance->window_styles[0]->broken_in_window_skel_data);
		buildings[i]->broken_iwindow_skel->parent = buildings[i];
		buildings[i]->broken_owindow_skel = new Skeleton(Global_Tiles::instance->window_styles[0]->broken_out_window_skel_data);
		buildings[i]->broken_owindow_skel->parent = buildings[i];
	}

	reset();

	//Setting up one-time global parameters
	float width = (float) screen_width;
	float height = (float)screen_height;
	Shader::set_static_global_param(Shader::GLOBAL_PARAM_FLOAT_WIDTH, &width);
	Shader::set_static_global_param(Shader::GLOBAL_PARAM_FLOAT_HEIGHT, &height);
}

//Prepares the game to run again
void Game::reset()
{
	for(int i = 0; i < MAX_BUILDINGS; i++)
	{
		buildings[i]->clear();
	}

	buildings[0]->generate(NULL,Vec3::ZERO());

	//Distance between buildings
	Vec3 bldg_offset = Vec3(0,BUILDING_GAP,0);

	for(int i = 1; i < MAX_BUILDINGS; i++)
	{
		buildings[i]->generate(buildings[PREV_BLDG[i]],bldg_offset);
	}

	current_building = buildings[0];
	cbldg_index = 0;

	//Place the player in the tile closest to the center of the first building
	player->pos.x = current_building->global_mins.x;
	player->pos.x += floorf(current_building->dimensions.x / 2.0f) * TILE_SIZE + (0.5f * TILE_SIZE);
	player->pos.y = current_building->global_mins.y + 1.0f;
	player->pos.z = BUILDING_GROUNDLEVEL + 15 * WINDOW_TILE_SIZE;

	player_head->viewbob_angles = Vec3(0,0,0);
	player_head->viewbob_vel = Vec3(0,0,0);

	if(office_ambience_src)
	{
		office_ambience_src->stop_audio();
	}
	office_ambience_src = Audio_Engine::play_sound(snd_office_amb,NULL,player->pos,0,0.6f,SOUND_END_TYPE_LOOP);

	if(highrise_ambience_src)
	{
		highrise_ambience_src->stop_audio();
	}
	highrise_ambience_src = Audio_Engine::play_sound(snd_highrise_amb,NULL,player->pos + Vec3(0,10.0f,0),0,1.0f,SOUND_END_TYPE_LOOP);



	//Generate the first building's floor after we generate all of the buildings themselves
	buildings[0]->generate_floor(player->pos,buildings[1]);

	player_state = PLAYER_STATE_NOCLIP;

	if(player_breath_src)
	{
		player_breath_src->stop_audio();
	}
	player_breath_src = player->play_sound(snd_breath,Vec3(0,0,0),0.6f,SOUND_END_TYPE_LOOP);

	player_phys_vel = Vec3::ZERO();
	cplayer_col_precedence_array = running_col_precedence;

	last_recycled_bldg_index = MAX_BUILDINGS - 1;
	bldgs_jumped = 0;
	recycle_every_time = false;
}

//Ran on last frame
//This is where we destroy our game objects
void Game::finish()
{
	for(int i = 0; i < MAX_BUILDINGS; i++)
	{
		buildings[i]->clear();
		delete buildings[i]->broken_owindow_skel;
		delete buildings[i]->broken_iwindow_skel;
		delete buildings[i];
	}

	Global_Tiles::term_data();

	delete[] buildings;

	delete player;
	delete player_skel;
	delete player_head;
	delete camera;
	delete cam_to_bone;
}

//==== Game logic methods =====

//Draws player bounding box
void Game::draw_player_bbox(Mat4 vp)
{
	//Drawing bounding box
	const float bounding_box[] =
	{
		//Box portion
		//Bottom of box
		-PLAYER_SIZE,-PLAYER_SIZE,0.0f,	-PLAYER_SIZE,PLAYER_SIZE,0.0f,
		-PLAYER_SIZE,PLAYER_SIZE,0.0f,	PLAYER_SIZE,PLAYER_SIZE,0.0f,
		PLAYER_SIZE,PLAYER_SIZE,0.0f,		PLAYER_SIZE,-PLAYER_SIZE,0.0f,
		PLAYER_SIZE,-PLAYER_SIZE,0.0f,	-PLAYER_SIZE,-PLAYER_SIZE,0.0f,
		//Top of box
		-PLAYER_SIZE,-PLAYER_SIZE,2.0f,	-PLAYER_SIZE,PLAYER_SIZE,2.0f,
		-PLAYER_SIZE,PLAYER_SIZE,2.0f,	PLAYER_SIZE,PLAYER_SIZE,2.0f,
		PLAYER_SIZE,PLAYER_SIZE,2.0f,		PLAYER_SIZE,-PLAYER_SIZE,2.0f,
		PLAYER_SIZE,-PLAYER_SIZE,2.0f,	-PLAYER_SIZE,-PLAYER_SIZE,2.0f,
		//Corners of box
		-PLAYER_SIZE,-PLAYER_SIZE,0.0f,	-PLAYER_SIZE,-PLAYER_SIZE,2.0f,
		-PLAYER_SIZE,PLAYER_SIZE,0.0f,	-PLAYER_SIZE,PLAYER_SIZE,2.0f,
		PLAYER_SIZE,PLAYER_SIZE,0.0f,		PLAYER_SIZE,PLAYER_SIZE,2.0f,
		PLAYER_SIZE,-PLAYER_SIZE,0.0f,	PLAYER_SIZE,-PLAYER_SIZE,2.0f,

		//Triangle portion
		//Bottom of triangle
		-PLAYER_SIZE,PLAYER_SIZE,0.0f,	0.0f,PLAYER_SIZE+player_bbox_tri_height,0.0f,
		PLAYER_SIZE,PLAYER_SIZE,0.0f,		0.0f,PLAYER_SIZE+player_bbox_tri_height,0.0f,
		//Top of triangle
		-PLAYER_SIZE,PLAYER_SIZE,2.0f,	0.0f,PLAYER_SIZE+player_bbox_tri_height,2.0f,
		PLAYER_SIZE,PLAYER_SIZE,2.0f,		0.0f,PLAYER_SIZE+player_bbox_tri_height,2.0f,
		//Edge of triangle
		0.0f,PLAYER_SIZE+player_bbox_tri_height,0.0f,	0.0f,PLAYER_SIZE+player_bbox_tri_height,2.0f,

		//Axes at player position
		//X-axis
		0.0f,0.0f,0.0f,	0.25f,0.0f,0.0f,
		//Y-axis
		0.0f,0.0f,0.0f,	0.0f,0.25f,0.0f,
		//Z-axis
		0.0f,0.0f,0.0f,	0.0f,0.0f,0.25f,
	};

	solid_mat->bind_material();
	Mat4 bbox_pos = vp * Mat4::TRANSLATE(player->pos);
	solid_mat->bind_value(Shader::PARAM_MVP_MATRIX,(void*) bbox_pos.m);

	float color_green[] = {0.0f,1.0f,0.0f,1.0f};
	float color_red[] = {1.0f,0.0f,0.0f,1.0f};



	solid_mat->bind_value(Shader::PARAM_VERTICES,(void*) bounding_box);

	//Check if player is colliding:
	move_player(Vec3::ZERO());
	//player_colliding is updated by this call (which does nothing)
	if(!player_colliding)
		solid_mat->bind_value(Shader::PARAM_COLOR_ADD,(void*) color_green);
	else
		solid_mat->bind_value(Shader::PARAM_COLOR_ADD,(void*) color_red);
	glDrawArrays(GL_LINES, 0, 40);
}

//Draws the screen overlay (a slight edge darkening on the screen)
//This also handles the black / white overlays on the screen
void Game::render_screen_overlay()
{
	const float quad_verts[] =
	{
	-1.0f,1.0f,0.3f,
	-1.0f,-1.0f,0.3f,
	1.0f,-1.0f,0.3f,
	1.0f,-1.0f,0.3f,
	1.0f,1.0f,0.3f,
	-1.0f,1.0f,0.3f
	};

	const float info[] = { black_overlay_opacity, white_overlay_opacity, edge_darken_opacity,0.0f };

	screen_overlay_mat->bind_material();
	screen_overlay_mat->bind_value(Shader::PARAM_VERTICES,(void*) quad_verts);
	screen_overlay_mat->bind_value(Shader::PARAM_COLOR_MULT, (void *) info);

	glDrawArrays(GL_TRIANGLES, 0, 6);
}

//Draws active floor maneuver keyframes
void Game::draw_keyframe(Mat4 vp, Keyframe* key, Vec3 ofs)
{
	Vec3 mins = key->mins;
	Vec3 maxs = key->maxs;
	const float bounds[] =
	{
		//Bottom box
		mins.x,mins.y,mins.z,		mins.x,maxs.y,mins.z,
		mins.x,maxs.y,mins.z,		maxs.x,maxs.y,mins.z,
		maxs.x,maxs.y,mins.z,		maxs.x,mins.y,mins.z,
		maxs.x,mins.y,mins.z,		mins.x,mins.y,mins.z,
		//Top box
		mins.x,mins.y,maxs.z,		mins.x,maxs.y,maxs.z,
		mins.x,maxs.y,maxs.z,		maxs.x,maxs.y,maxs.z,
		maxs.x,maxs.y,maxs.z,		maxs.x,mins.y,maxs.z,
		maxs.x,mins.y,maxs.z,		mins.x,mins.y,maxs.z,
		//bottom to top corners
		mins.x,mins.y,mins.z,		mins.x,mins.y,maxs.z,
		mins.x,maxs.y,mins.z,		mins.x,maxs.y,maxs.z,
		maxs.x,maxs.y,mins.z,		maxs.x,maxs.y,maxs.z,
		maxs.x,mins.y,mins.z,		maxs.x,mins.y,maxs.z,
	};
	const float point_bounds[] =
	{
		mins.x-0.25f,mins.y,mins.z,		mins.x+0.25f,mins.y,mins.z,
		mins.x,mins.y-0.25f,mins.z,		mins.x,mins.y+0.25f,mins.z,
		mins.x,mins.y,mins.z-0.25f,		mins.x,mins.y,mins.z+0.25f,
	};
	int vert_count = 0;

	solid_mat->bind_material();

	if(mins.x == maxs.x && mins.y == maxs.y)
	{
		//bind axes
		solid_mat->bind_value(Shader::PARAM_VERTICES,(void*) point_bounds);
		vert_count = 6;
	}
	else
	{
		//bind cube area
		solid_mat->bind_value(Shader::PARAM_VERTICES,(void*) bounds);
		vert_count = 24;
	}

	float color[] = {1.0f,1.0f,0.0f,1.0f};
	solid_mat->bind_value(Shader::PARAM_COLOR_ADD,(void*) color);

	Mat4 trans = vp * Mat4::TRANSLATE(ofs);
	solid_mat->bind_value(Shader::PARAM_MVP_MATRIX,(void*) trans.m);
	glDrawArrays(GL_LINES, 0, vert_count);
}
//Draws active floors maneuver data
void Game::draw_floor_maneuvers(Mat4 vp)
{
	Vec3 building_org = current_building->active_floor->global_mins + Vec3(0,0,0.5f)*GRID_SIZE;
	Vec3 tile_pos;

	for(int i = 0; i < current_building->active_floor->width; i++)
	{
		for(int j = 0; j < current_building->active_floor->length; j++)
		{
			tile_pos = building_org + Vec3(i*TILE_SIZE,j*TILE_SIZE,0.0f);
			if(current_building->active_floor->tile_object[i][j]->maneuver_count)
			{
				for(int k = 0; k < current_building->active_floor->tile_object[i][j]->maneuver_count; k++)
				{
					Maneuver* man = current_building->active_floor->tile_object[i][j]->maneuvers[k];
					Vec3 ofs = tile_pos + Vec3(0,0,0.1f);
					for(int l = 0; l < man->keyframe_count; l++)
					{
						draw_keyframe(vp,man->keyframes[l],ofs);
					}
				}
			}
		}
	}
}
//Draws active floors collision voxels
void Game::draw_floor_collision_voxels(Mat4 vp)
{
	Vec3 building_org = current_building->active_floor->global_mins + Vec3(0,0,0.5f)*GRID_SIZE;
	Vec3 tile_pos;
	Vec3 voxel_pos;
	char voxel_rank;
	char voxel_shape;

	//=========== Defining the different voxel shapes to render =============
	const float vshape_box[] =
	{
	//Top of box
	0,0,GRID_SIZE,			0,GRID_SIZE,GRID_SIZE,
	0,0,GRID_SIZE,			GRID_SIZE,0,GRID_SIZE,
	GRID_SIZE,0,GRID_SIZE,	GRID_SIZE,GRID_SIZE,GRID_SIZE,
	0,GRID_SIZE,GRID_SIZE,	GRID_SIZE,GRID_SIZE,GRID_SIZE,

	//Box x
	0,0,GRID_SIZE,			GRID_SIZE,GRID_SIZE,GRID_SIZE,
	GRID_SIZE,0,GRID_SIZE,	0,GRID_SIZE,GRID_SIZE
	};

	const float vshape_lt_pos[] =
	{
	0,0,GRID_SIZE,			GRID_SIZE,0,GRID_SIZE,
	GRID_SIZE,0,GRID_SIZE,	GRID_SIZE,GRID_SIZE,GRID_SIZE,
	0,0,GRID_SIZE,			GRID_SIZE,GRID_SIZE,GRID_SIZE
	};

	const float vshape_gt_pos[] =
	{
	0,0,GRID_SIZE,			0,GRID_SIZE,GRID_SIZE,
	0,GRID_SIZE,GRID_SIZE,	GRID_SIZE,GRID_SIZE,GRID_SIZE,
	0,0,GRID_SIZE,			GRID_SIZE,GRID_SIZE,GRID_SIZE
	};
	const float vshape_lt_neg[] =
	{
	0,0,GRID_SIZE,			0,GRID_SIZE,GRID_SIZE,
	0,0,GRID_SIZE,			GRID_SIZE,0,GRID_SIZE,
	GRID_SIZE,0,GRID_SIZE,	0,GRID_SIZE,GRID_SIZE
	};
	const float vshape_gt_neg[] =
	{
	GRID_SIZE,0,GRID_SIZE,	GRID_SIZE,GRID_SIZE,GRID_SIZE,
	0,GRID_SIZE,GRID_SIZE,	GRID_SIZE,GRID_SIZE,GRID_SIZE,
	GRID_SIZE,0,GRID_SIZE,	0,GRID_SIZE,GRID_SIZE
	};
	const float vshape_gt_abs[] =
	{
	0,GRID_SIZE,GRID_SIZE,			GRID_SIZE*0.5f,GRID_SIZE*0.5f,GRID_SIZE,
	GRID_SIZE,GRID_SIZE,GRID_SIZE,	GRID_SIZE*0.5f,GRID_SIZE*0.5f,GRID_SIZE,
	0,GRID_SIZE,GRID_SIZE,			GRID_SIZE,GRID_SIZE,GRID_SIZE
	};
	const float vshape_lt_abs[] =
	{
	0,0,GRID_SIZE,			GRID_SIZE*0.5f,GRID_SIZE*0.5f,GRID_SIZE,
	GRID_SIZE,0,GRID_SIZE,		GRID_SIZE*0.5f,GRID_SIZE*0.5f,GRID_SIZE,
	0,0,GRID_SIZE,			GRID_SIZE,0,GRID_SIZE
	};

	float wall_ofs = 0.15;
	const float vshape_in_wall_pos[] =
	{
	0,0,GRID_SIZE,							wall_ofs,0,GRID_SIZE,
	0,0,GRID_SIZE,							0,wall_ofs,GRID_SIZE,
	wall_ofs,0,GRID_SIZE,					GRID_SIZE,GRID_SIZE-wall_ofs,GRID_SIZE,
	0,wall_ofs,GRID_SIZE,					GRID_SIZE-wall_ofs,GRID_SIZE,GRID_SIZE,
	GRID_SIZE,GRID_SIZE-wall_ofs,GRID_SIZE,		GRID_SIZE,GRID_SIZE,GRID_SIZE,
	GRID_SIZE-wall_ofs,GRID_SIZE,GRID_SIZE,		GRID_SIZE,GRID_SIZE,GRID_SIZE
	};
	const float vshape_in_wall_neg[] =
	{
	0,GRID_SIZE,GRID_SIZE,			wall_ofs,GRID_SIZE,GRID_SIZE,
	0,GRID_SIZE,GRID_SIZE,			0,GRID_SIZE-wall_ofs,GRID_SIZE,
	wall_ofs,GRID_SIZE,GRID_SIZE,		GRID_SIZE,wall_ofs,GRID_SIZE,
	0,GRID_SIZE-wall_ofs,GRID_SIZE,	GRID_SIZE-wall_ofs,0,GRID_SIZE,
	GRID_SIZE,wall_ofs,GRID_SIZE,		GRID_SIZE,0,GRID_SIZE,
	GRID_SIZE-wall_ofs,0,GRID_SIZE,	GRID_SIZE,0,GRID_SIZE
	};

	const GLsizei vshape_vert_counts[9] = {12,6,6,6,6,6,6,12,12};

	const float* voxel_shapes[9];
	voxel_shapes[CLIP_SHAPE_BOX] = vshape_gt_pos;//vshape_box;
	voxel_shapes[CLIP_SHAPE_LT_POS] = vshape_lt_pos;
	voxel_shapes[CLIP_SHAPE_GT_POS] = vshape_gt_pos;
	voxel_shapes[CLIP_SHAPE_LT_NEG] = vshape_lt_neg;
	voxel_shapes[CLIP_SHAPE_GT_NEG] = vshape_gt_neg;
	voxel_shapes[CLIP_SHAPE_GT_ABS] = vshape_gt_abs;
	voxel_shapes[CLIP_SHAPE_LT_ABS] = vshape_lt_abs;
	voxel_shapes[CLIP_SHAPE_IN_WALL_POS] = vshape_in_wall_pos;
	voxel_shapes[CLIP_SHAPE_IN_WALL_NEG] = vshape_in_wall_neg;

	//=======================================================================
	//Different Voxel Colors

	const float color_red[] = {1.0f,0.0f,0.0f,1.0f};
	const float color_purple[] = {1.0f,0.0f,1.0f,1.0f};
	const float color_dark_blue[] = {0.0f,0.0f,0.5f,1.0f};
	const float color_dark_red[] = {0.5f,0.0f,0.0f,1.0f};
	const float color_dark_gray[] = {0.2f,0.2f,0.2f,1.0f};

	const float* voxel_types[5];

	voxel_types[CLIP_EMPTY] = color_dark_gray;
	voxel_types[CLIP_SOLID] = color_purple;
	voxel_types[CLIP_SLIDE] = color_red;
	voxel_types[CLIP_DOORWAY] = color_dark_red;
	voxel_types[CLIP_MID] = color_dark_blue;


	solid_mat->bind_material();
	Mat4 vox_trans;

	Voxel vox;

	for(int i = 0; i < current_building->active_floor->width; i++)
	{
		for(int j = 0; j < current_building->active_floor->length; j++)
		{
			tile_pos = building_org + Vec3(i*TILE_SIZE,j*TILE_SIZE,0.0f);
			for(int k = 0; k < TILE_VOXEL_DIMS; k++)
			{
				for(int l = 0; l < TILE_VOXEL_DIMS; l++)
				{
					vox = current_building->active_floor->tile_coll_map[i][j]->get_vox_at(k,l);
					voxel_rank = vox.clip_type;
					voxel_shape = vox.clip_shape;

					if(voxel_rank == CLIP_EMPTY)
					{
						continue;
					}
					solid_mat->bind_value(Shader::PARAM_VERTICES,(void*) voxel_shapes[voxel_shape]);
					solid_mat->bind_value(Shader::PARAM_COLOR_ADD,(void*) voxel_types[voxel_rank]);
					voxel_pos = tile_pos + Vec3(k*GRID_SIZE,l*GRID_SIZE,0.0f);
					vox_trans = vp * Mat4::TRANSLATE(voxel_pos);
					solid_mat->bind_value(Shader::PARAM_MVP_MATRIX,(void*) vox_trans.m);
					glDrawArrays(GL_LINES, 0, vshape_vert_counts[voxel_shape]);
				}
			}
		}
	}
}

//Returns the pos if in mins & maxs, otherwise caps to lie within the mins & maxs
Vec3 Game::cap_to_bounds(Vec3 pos, Vec3 mins, Vec3 maxs)
{
	Vec3 result = pos;
	//Check x-axis
	if(result.x <= mins.x)
		result.x = mins.x;
	else if(result.x >= maxs.x)
		result.x = maxs.x;
	//Check y-axis
	if(result.y <= mins.y)
		result.y = mins.y;
	else if(result.y >= maxs.y)
		result.y = maxs.y;
	//Check z-axis
	if(result.z <= mins.z)
		result.z = mins.z;
	else if(result.z >= maxs.z)
		result.z = maxs.z;

	return result;
}

void Game::mnvr_movement()
{
	mnvr_y_vel += mnvr_frame->y_accel * Time::udelta_time;
	if(mnvr_y_vel <= mnvr_frame->min_y_vel)
	{
		mnvr_y_vel = mnvr_frame->min_y_vel;
	}

	player->pos.y += mnvr_y_vel * Time::udelta_time;

	//Lerping x & z player coordinates

	//Do not overshoot the goal
	float temp_y = player->pos.y > mnvr_goal_pos.y ? mnvr_goal_pos.y : player->pos.y;

	//Lerp the player x coordinate:
	player->pos.x = mnvr_var_x_slope * (temp_y - mnvr_goal_pos.y) + mnvr_goal_pos.x;

	//Lerping the player Z coordinate:
	switch(mnvr_frame->lerp_type)
	{
		default:
		case FRAME_LERP_LINEAR:
			player->pos.z = mnvr_var_a * (temp_y - mnvr_goal_pos.y) + mnvr_goal_pos.z;
			break;
		case FRAME_LERP_QUADRATIC:
			player->pos.z = (mnvr_var_a * temp_y * temp_y) + (mnvr_var_b * temp_y) + mnvr_var_c;
			break;
		case FRAME_LERP_QUAD_TO_VERT:
			player->pos.z = (mnvr_var_a * (temp_y - mnvr_goal_pos.y)*(temp_y - mnvr_goal_pos.y)) + mnvr_goal_pos.z;
			break;
		case FRAME_LERP_QUAD_FROM_VERT:
			player->pos.z = (mnvr_var_a * (temp_y - mnvr_start_pos.y)*(temp_y - mnvr_start_pos.y)) + mnvr_start_pos.z;
			break;
	}

	//Turning the player to have the right orientation
	if(mnvr_frame->orient == FRAME_ORIENT_CONSTANT)
		mnvr_goal_yaw_rot = get_keyframe_goal_yaw(mnvr_frame),player->angles.y;



	//Slight camera roll rotation when turning
	float tilt_angle = (player->angles.y - mnvr_goal_yaw_rot) * 0.8f;
	tilt_angle = efmodf(tilt_angle + PI,TWO_PI) - PI;
	player_head->tilt_angles.z = lerp_wtd_avg(player_head->tilt_angles.z,tilt_angle,5.0f);

	player->angles.y += (mnvr_goal_yaw_rot - player->angles.y) * mnvr_frame->orient_speed;

	if(mnvr_current->end_on_anim_end)
	{
		//Check if we are on the last frame
		if(mnvr_frame_number >= mnvr_current->keyframe_count - 1)
		{
			if(!player_skel->playing_anim)
			{
				reached_mnvr_keyframe();
				return;
			}
		}
	}

	if(player->pos.y > mnvr_goal_pos.y)
	{
		reached_mnvr_keyframe();
	}
}

//Returns angle from player pos to the orientation position of the keyframe
float Game::get_keyframe_goal_yaw(Keyframe* key)
{
	float yaw;

	yaw = -atanf((mnvr_tile_ofs.x + key->orient_pos.x - player->pos.x)/(mnvr_tile_ofs.y + key->orient_pos.y - player->pos.y));
	//if the face position is behind the player, adjust results of atan
	if(player->pos.y > mnvr_tile_ofs.y + key->orient_pos.y)
		yaw += PI;

	//return angle closest to the player's yaw
	yaw = closest_angle(yaw,player->angles.y);
	return yaw;
}

void Game::reached_mnvr_keyframe ()
{
	//If we are traversing: handle special flags
	if(player_state == PLAYER_STATE_TRAVERSING && mnvr_frame_number >= 0)
	{
		switch(mnvr_next_frame->spec_flag)
		{
			default:
				break;
			case FRAME_SPECFLAG_BREAKWINDOW_OUT:
				current_building->break_window(player->pos,false);
				player->play_sound(snd_winbreak_out,Vec3(0,0,0),0.8f,SOUND_END_TYPE_STOP);
				player->play_sound(snd_jumpwind,Vec3(0,0,0),1.0f,SOUND_END_TYPE_STOP);
				//If we are leaving the middle building, recycle a building
				if(bldgs_jumped >= MAX_BUILDINGS/2 || recycle_every_time)
				{
					recycle_every_time = true;
					//Getting the building that we are recycling
					int bldg_index = NEXT_BLDG[last_recycled_bldg_index];
					buildings[bldg_index]->clear();
					buildings[bldg_index]->generate(buildings[last_recycled_bldg_index],Vec3(0,BUILDING_GAP,0));
					last_recycled_bldg_index = bldg_index;


					//Reset everything at origin
					Vec3 ofs = Vec3(-player->pos.x,-player->pos.y,0);
					for(int i = 0; i < MAX_BUILDINGS; i++)
					{
						buildings[i]->offset_building(ofs);
					}
					player->pos += ofs;
					mnvr_tile_ofs += ofs;
					mnvr_goal_pos += ofs;
					mnvr_start_pos += ofs;
				}
				bldgs_jumped++;

				break;
			case FRAME_SPECFLAG_BREAKWINDOW_IN:
			{
				//At this point the next building will be our current building
				int next_bldg_index = NEXT_BLDG[cbldg_index];
				Building* next_bldg = buildings[next_bldg_index];
				current_building = next_bldg;
				cbldg_index = next_bldg_index;

				int floor = current_building->get_floor_num_at_pos(player->pos);
				if(floor < LOWEST_FLOOR_ALLOWED)
				{
					//Reset player at 4 floors below top floor
					Vec3 ofs = Vec3(0,0,(current_building->dimensions.z - 4 - floor) * WINDOW_TILE_SIZE);
					player->pos += ofs;
					mnvr_tile_ofs += ofs;
					mnvr_goal_pos += ofs;
					mnvr_start_pos += ofs;
				}

				current_building->generate_floor(player->pos,buildings[NEXT_BLDG[cbldg_index]]);
				current_building->break_window(player->pos,true);
				player->play_sound(snd_winbreak_in,Vec3(0,0,0),0.8f,SOUND_END_TYPE_STOP);
				break;
			}
		}
	}
	//Setting up movement for keyframe:
	mnvr_frame_number = mnvr_next_frame_number;
	mnvr_next_frame_number++;
	//Are we done with this maneuver?
	if(mnvr_next_frame_number >= mnvr_current->keyframe_count)
	{
		player_state = PLAYER_STATE_RUNNING;
		mnvr_current = NULL;
		mnvr_frame = NULL;
		mnvr_next_frame = NULL;
		mnvr_frame_number = 0;
		mnvr_next_frame_number = 0;
	}
	else
	{
		mnvr_frame = mnvr_current->keyframes[mnvr_frame_number];
		mnvr_next_frame = mnvr_current->keyframes[mnvr_next_frame_number];

		//Handling animation
		switch(mnvr_frame->anim_cmd)
		{
			default:
			case FRAME_ANIM_NOOP:
				break;
			case FRAME_ANIM_PLAY:
				player_skel->play_anim(mnvr_frame->anim,mnvr_frame->anim_end_type);
				break;
			case FRAME_ANIM_PAUSE:
				player_skel->pause_anim();
				break;
			case FRAME_ANIM_RESUME:
				player_skel->resume_anim();
				break;
			case FRAME_ANIM_STOP:
				player_skel->stop_anim();
				break;
			case FRAME_ANIM_PLAY_DEFAULT:
				player_skel->play_default_anim();
				break;
		}

		//Setting up movement data for the keyframe
		mnvr_start_pos = player->pos;
		mnvr_goal_pos = cap_to_bounds(player->pos, mnvr_tile_ofs + mnvr_next_frame->mins, mnvr_tile_ofs + mnvr_next_frame->maxs);

		//Check if we are already past our goal position
		if(mnvr_start_pos.y >= mnvr_goal_pos.y)
		{
			reached_mnvr_keyframe();
			return;
		}

		if(mnvr_frame->orient)
		{
			mnvr_goal_yaw_rot = get_keyframe_goal_yaw(mnvr_frame);
		}
		else
			mnvr_goal_yaw_rot = 0.0f;

		mnvr_y_vel = mnvr_frame->y_vel;
		//Adjust the speed so we reach the next keyframe in the same amount of time regardless of where in the keyframe bounds we are
		if(mnvr_frame->speed_type == FRAME_SPEED_CONST_TIME)
		{
			//find distance to our goal pos from the center of this keyframe
			float d_from_keyframe_center = mnvr_next_frame->mins.y - ((mnvr_frame->mins.y + mnvr_frame->maxs.y)/2.0f);
			//Calculate how long that would take to travel at mnvr_y_vel m/s
			float t_from_keyframe_center = d_from_keyframe_center / mnvr_y_vel;
			//Calculate our speed if we want to get there in the same amonut of time from our position
			mnvr_y_vel = (mnvr_goal_pos.y - mnvr_start_pos.y)/t_from_keyframe_center;
		}

		mnvr_var_x_slope = ((mnvr_goal_pos.x - mnvr_start_pos.x)/(mnvr_goal_pos.y - mnvr_start_pos.y));

		//Calculating the different lerp parameters needed
		switch(mnvr_frame->lerp_type)
		{
			default:
			case FRAME_LERP_LINEAR:
				mnvr_var_a = ((mnvr_goal_pos.z - mnvr_start_pos.z)/(mnvr_goal_pos.y - mnvr_start_pos.y));
				break;
			case FRAME_LERP_QUADRATIC:
				mnvr_var_a = mnvr_frame->lerp_data / (2 * mnvr_y_vel * mnvr_y_vel);
				mnvr_var_b = ((mnvr_var_a * mnvr_start_pos.y * mnvr_start_pos.y)
								- (mnvr_var_a * mnvr_goal_pos.y * mnvr_goal_pos.y)
								+ (mnvr_goal_pos.z - mnvr_start_pos.z))
							/(mnvr_goal_pos.y - mnvr_start_pos.y);
				mnvr_var_c = mnvr_start_pos.z - (mnvr_var_a * mnvr_start_pos.y * mnvr_start_pos.y) - (mnvr_var_b * mnvr_start_pos.y);
				break;
			case FRAME_LERP_QUAD_TO_VERT:
				mnvr_var_a = (mnvr_start_pos.z - mnvr_goal_pos.z)
							/ ((mnvr_start_pos.y - mnvr_goal_pos.y) * (mnvr_start_pos.y - mnvr_goal_pos.y));
				break;
			case FRAME_LERP_QUAD_FROM_VERT:
				mnvr_var_a = (mnvr_goal_pos.z - mnvr_start_pos.z)
							/ ((mnvr_goal_pos.y - mnvr_start_pos.y) * (mnvr_goal_pos.y - mnvr_start_pos.y));
				break;
		}
	}
}

//Returns true if all points (2d coords) in pts are on the left side of line l1->l2
//(left is defined from standing at l1, looking at l2)
bool Game::clip_all_on_left_side(Vec3 l1, Vec3 l2,float *pts, int pt_count)
{
	float pt_x;
	float pt_y;

	for(int i = 0; i < pt_count; i++)
	{
		pt_x = pts[2*i];
		pt_y = pts[2*i + 1];

		//Calculating what side the point (pt_x,pt_y) is on
		float temp = (l2.x-l1.x)*(pt_y-l1.y) - (l2.y-l1.y)*(pt_x-l1.x);

		//If temp is > 0, (pt_x,pt_y) is to the left of the line
		//If temp is < 0, (pt_x,pt_y) is to the right of the line
		//If temp is == 0, (pt_x,pt_y) is on the line
		if(temp <= 0)
			return false;
	}
	return true;
}

//Given portion of bbox outlined by (la,lb), check if vox is outside of the bbox line
char Game::voxel_not_in_line(Vec3 la, Vec3 lb, Voxel vox)
{
	//Check if every vertex that defines the voxel shape is to the left of the line (la,lb)
	bool not_touching = clip_all_on_left_side(la,lb,CLIP_SHAPE_SHAPES[vox.clip_shape],CLIP_SHAPE_VERT_COUNTS[vox.clip_shape]);

	if(!not_touching)
	{
		return vox.clip_type;
	}
	return CLIP_EMPTY;
}

//Given portion of bbox outlined by both (l1a,l1b) and (l2a,l2b), , check if vox is outside of both bbox lines
char Game::voxel_not_in_lines(Vec3 l1a, Vec3 l1b, Vec3 l2a, Vec3 l2b, Voxel vox)
{
	//Check if every vertex that defines the voxel shape is to the left of both lines (l1a,l1b) and (l2a,l2b)
	bool not_touching1 = clip_all_on_left_side(l1a,l1b,CLIP_SHAPE_SHAPES[vox.clip_shape],CLIP_SHAPE_VERT_COUNTS[vox.clip_shape]);
	bool not_touching2 = clip_all_on_left_side(l2a,l2b,CLIP_SHAPE_SHAPES[vox.clip_shape],CLIP_SHAPE_VERT_COUNTS[vox.clip_shape]);

	if(!not_touching1 && !not_touching2)
	{
		return vox.clip_type;
	}

	return CLIP_EMPTY;
}


//Returns whether or not the collision player bbox is colliding with something at position p
//Sets col_dir and col_type to
bool Game::clip_player_bbox(Vec3 p, char &col_dir, char &col_type)
{
	//check for collisions at certain points
	Vec3 floor_pos = p - current_building->active_floor->global_mins;
	Voxel vox;

	//==============================================================================
	//Checking if any part of the bounding box is out of bounds
	//Checking if triangle tip is out of bounds in y-axis
	if(current_building->active_floor->is_y_out_of_bounds(p+Vec3(0,PLAYER_SIZE+player_bbox_tri_height,0)))
	{
		col_dir = COL_DIR_FORWARD;
		col_type = CLIP_WINDOW;
		return true;
	}
	//Checking if bbox back midpoint is out of bounds in y-axis
	if(current_building->active_floor->is_y_out_of_bounds(p+Vec3(0,PLAYER_SIZE+player_bbox_tri_height,0)))
	{
		col_dir = COL_DIR_FORWARD;
		//We don't care about breaking out of a window towards the back side
		col_type = CLIP_SOLID;
		return true;
	}
	//Checking if bbox right midpoint is out of bounds in x-axis
	if(current_building->active_floor->is_x_out_of_bounds(p+Vec3(PLAYER_SIZE,0,0)))
	{
		col_dir = COL_DIR_RIGHT;
		col_type = CLIP_WINDOW;
		return true;
	}
	//Checking if bbox left midpoint is out of bounds in x-axis
	if(current_building->active_floor->is_x_out_of_bounds(p-Vec3(PLAYER_SIZE,0,0)))
	{
		col_dir = COL_DIR_LEFT;
		col_type = CLIP_WINDOW;
		return true;
	}
	//==============================================================================



	//Front of triangle
	float mod_x = efmodf(floor_pos.x,GRID_SIZE);
	float mod_y = efmodf(floor_pos.y,GRID_SIZE);

	float rcorner_cmpr = -player_bbox_tri_slope * (mod_x - (GRID_SIZE-PLAYER_SIZE)) + (GRID_SIZE-PLAYER_SIZE);
	float lcorner_cmpr = player_bbox_tri_slope * (mod_x - PLAYER_SIZE) + (GRID_SIZE-PLAYER_SIZE);
	float max_cmpr = GRID_SIZE - PLAYER_SIZE;

	//The points that make up the player collision prediction triangle (relative to the player center)
	Vec3 col_tri_tip = Vec3(0,PLAYER_SIZE + player_bbox_tri_height,0);

	//Points making up bbox, relative to top left (to match triangle reference point that we just outlined)
	Vec3 bbox_tl = Vec3(-PLAYER_SIZE,PLAYER_SIZE,0.0f);
	Vec3 bbox_tr = Vec3(PLAYER_SIZE,PLAYER_SIZE,0.0f);
	Vec3 bbox_bl = Vec3(-PLAYER_SIZE,-PLAYER_SIZE,0.0f);
	Vec3 bbox_br = Vec3(PLAYER_SIZE,-PLAYER_SIZE,0.0f);
	//ofs will be defined per-voxel as the position of top left corner of the bbox relative to the voxel position (when mod_x and mod_y are both zero)
	Vec3 ofs;

	//This is a list of what voxels each check hit
	char front_clip_results[5] = {0,0,0,0,0};

	//======= A bunch of const identifiers for readability =======
	const int clip_rslt_tri_tip = 0;
	const int clip_rslt_lcorner = 1;
	const int clip_rslt_rcorner = 2;
	const int clip_rslt_fl = 3;
	const int clip_rslt_fr = 4;
	//============================================================
	Vec3 test_point;
	//Position of the voxel within the floor
	Vec3 voxel_floor_pos;

	//If our prediction triangle is intersecting the front left voxel: consider it
	if(mod_y >= lcorner_cmpr && mod_y <= max_cmpr)
	{
		//Check front left voxel
		test_point = floor_pos + Vec3(-PLAYER_SIZE,PLAYER_SIZE + GRID_SIZE,0);
		voxel_floor_pos = Vec3(test_point.x - fmodf(test_point.x,GRID_SIZE),test_point.y - fmodf(test_point.y,GRID_SIZE),0);
		vox = current_building->get_voxel_at(test_point);
		if(vox.clip_type != CLIP_EMPTY)
		{
			ofs = floor_pos - voxel_floor_pos;
			front_clip_results[clip_rslt_lcorner] = voxel_not_in_line(bbox_tl + ofs, col_tri_tip + ofs, vox);
		}
	}
	//If our prediction triangle is intersecting the front right voxel: consider it
	if(mod_y >= rcorner_cmpr && mod_y <= max_cmpr)
	{
		//Check front right voxel
		test_point = floor_pos + Vec3(PLAYER_SIZE,PLAYER_SIZE + GRID_SIZE,0);
		voxel_floor_pos = Vec3(test_point.x - fmodf(test_point.x,GRID_SIZE),test_point.y - fmodf(test_point.y,GRID_SIZE),0);
		vox = current_building->get_voxel_at(test_point);
		//If the voxel is not empty
		if(vox.clip_type != CLIP_EMPTY)
		{
			ofs = floor_pos - voxel_floor_pos;
			front_clip_results[clip_rslt_rcorner] = voxel_not_in_line(col_tri_tip + ofs, bbox_tr + ofs, vox);
		}
	}

	//Triangle tip
	test_point = floor_pos + Vec3(0,PLAYER_SIZE + player_bbox_tri_height,0);
	voxel_floor_pos = Vec3(test_point.x - fmodf(test_point.x,GRID_SIZE),test_point.y - fmodf(test_point.y,GRID_SIZE),0);
	vox = current_building->get_voxel_at(test_point);
	if(vox.clip_type != CLIP_EMPTY)
	{
		ofs = floor_pos - voxel_floor_pos;
		front_clip_results[clip_rslt_tri_tip] = voxel_not_in_lines(bbox_tl + ofs, col_tri_tip + ofs, col_tri_tip + ofs, bbox_tr + ofs,vox);
	}

	//Front right
	test_point = floor_pos + Vec3(PLAYER_SIZE,PLAYER_SIZE,0);
	voxel_floor_pos = Vec3(test_point.x - fmodf(test_point.x,GRID_SIZE),test_point.y - fmodf(test_point.y,GRID_SIZE),0);
	vox = current_building->get_voxel_at(test_point);
	if(vox.clip_type != CLIP_EMPTY)
	{
		ofs = floor_pos - voxel_floor_pos;
		front_clip_results[clip_rslt_fr] = voxel_not_in_lines(col_tri_tip + ofs, bbox_tr + ofs, bbox_tr + ofs, bbox_br + ofs,vox);
	}

	//Front left
	test_point = floor_pos + Vec3(-PLAYER_SIZE,PLAYER_SIZE,0);
	voxel_floor_pos = Vec3(test_point.x - fmodf(test_point.x,GRID_SIZE),test_point.y - fmodf(test_point.y,GRID_SIZE),0);
	vox = current_building->get_voxel_at(test_point);
	if(vox.clip_type != CLIP_EMPTY)
	{
		ofs = floor_pos - voxel_floor_pos;
		front_clip_results[clip_rslt_fl] = voxel_not_in_lines(bbox_bl + ofs, bbox_tl + ofs, bbox_tl + ofs, col_tri_tip + ofs,vox);
	}

	//Return the highest rank voxel that we collided into on the front.
	char highest_clip_type = CLIP_EMPTY;
	char highest_clip_priority = CLIP_EMPTY;
	for(int i = 0; i < 5; i++)
	{
		//Using precedence array to get the priority of the collision clip type
		if(cplayer_col_precedence_array[front_clip_results[i]] > highest_clip_priority)
		{
			highest_clip_priority = cplayer_col_precedence_array[front_clip_results[i]];
			highest_clip_type = front_clip_results[i];
		}
	}

	if(highest_clip_type)
	{
		col_dir = COL_DIR_FORWARD;
		col_type = highest_clip_type;
		return true;
	}

	//Mid right
	test_point = floor_pos + Vec3(PLAYER_SIZE,0,0);
	voxel_floor_pos = Vec3(test_point.x - fmodf(test_point.x,GRID_SIZE),test_point.y - fmodf(test_point.y,GRID_SIZE),0);
	vox = current_building->get_voxel_at(test_point);
	if(vox.clip_type != CLIP_EMPTY)
	{
		ofs = floor_pos - voxel_floor_pos;
		highest_clip_type = voxel_not_in_line(bbox_tr + ofs, bbox_br + ofs,vox);
		highest_clip_priority = cplayer_col_precedence_array[highest_clip_type];
		if(highest_clip_priority)
		{
			col_dir = COL_DIR_RIGHT;
			col_type = highest_clip_type;
			return true;
		}
	}

	//Mid left
	test_point = floor_pos + Vec3(-PLAYER_SIZE,0,0);
	voxel_floor_pos = Vec3(test_point.x - fmodf(test_point.x,GRID_SIZE),test_point.y - fmodf(test_point.y,GRID_SIZE),0);
	vox = current_building->get_voxel_at(test_point);
	if(vox.clip_type != CLIP_EMPTY)
	{
		ofs = floor_pos - voxel_floor_pos;
		highest_clip_type = voxel_not_in_line(bbox_bl + ofs, bbox_tl + ofs,vox);
		highest_clip_priority = cplayer_col_precedence_array[highest_clip_type];
		if(highest_clip_priority)
		{
			col_dir = COL_DIR_LEFT;
			col_type = highest_clip_type;
			return true;
		}
	}

	//We're going to skip these collision checks, I feel they are just nitpicking and their removal could make the game easier
	//Back center
	/*test_point = floor_pos + Vec3(0,-PLAYER_SIZE,0);
	voxel_floor_pos = Vec3(test_point.x - fmodf(test_point.x,GRID_SIZE),test_point.y - fmodf(test_point.y,GRID_SIZE),0);
	vox = current_building->get_voxel_at(test_point);
	if(vox.clip_type != CLIP_EMPTY)
	{
		ofs = floor_pos - voxel_floor_pos;
		clip_results[clip_rslt_bc] = voxel_not_in_line(bbox_br + ofs, bbox_bl + ofs,vox);
	}
	//back right
	test_point = floor_pos + Vec3(PLAYER_SIZE,-PLAYER_SIZE,0);
	voxel_floor_pos = Vec3(test_point.x - fmodf(test_point.x,GRID_SIZE),test_point.y - fmodf(test_point.y,GRID_SIZE),0);
	vox = current_building->get_voxel_at(test_point);
	if(vox.clip_type != CLIP_EMPTY)
	{
		ofs = floor_pos - voxel_floor_pos;
		clip_results[clip_rslt_br] = voxel_not_in_lines(bbox_tr + ofs, bbox_br + ofs, bbox_br + ofs, bbox_bl + ofs,vox);
	}
	//back left
	test_point = floor_pos + Vec3(-PLAYER_SIZE,-PLAYER_SIZE,0);
	voxel_floor_pos = Vec3(test_point.x - fmodf(test_point.x,GRID_SIZE),test_point.y - fmodf(test_point.y,GRID_SIZE),0);
	vox = current_building->get_voxel_at(test_point);
	if(vox.clip_type != CLIP_EMPTY)
	{
		ofs = floor_pos - voxel_floor_pos;
		clip_results[clip_rslt_bl] = voxel_not_in_lines(bbox_br + ofs, bbox_bl + ofs, bbox_bl + ofs, bbox_tl + ofs, vox);
	}*/

	return false;
}

//Returns if the pos's x coordinate is within the area required to start a building to building traversal
//This only checks the x-coordinate
bool Game::is_in_traversal_x_bounds(Vec3 pos)
{
	Vec3 tile_pos = Vec3(fmodf(pos.x,TILE_SIZE),fmodf(pos.y,TILE_SIZE),0);
	Vec3 mins;
	Vec3 maxs;
	Traversal* trav;
	for(int i = 0; i < BUILDING_TRAV_COUNT; i++)
	{
		trav = Global_Tiles::instance->bldg_travs[i];
		mins = trav->keyframes[0]->mins;
		maxs = trav->keyframes[0]->maxs;

		if(tile_pos.x >= mins.x && tile_pos.x <= maxs.x)
		{
			return true;
		}
	}
	return false;
}

//Plays a player death animation
//which animation depends on collision direction and collision type
void Game::start_player_death(char col_dir, char col_type)
{
	player_colliding = true;

	if(player_state == PLAYER_STATE_NOCLIP || player_state == PLAYER_STATE_CAM_FLY)
	{
		return;
	}

	bool player_sliding = (player_state == PLAYER_STATE_SLIDING);
	//When sliding, a small portion (frames 0 - 18) of the transition ( running -> sliding ) animation has the character upright
	//So we treat this segment as though the player is standing and play standing death animations
	if(player_sliding)
	{
		if(player_substate == 0.0f)
		{
			if(player_skel->current_frame <= 18)
			{
				player_sliding = false;
			}
		}
	}

	float t = Time::time();

	if(!player_sliding)
	{
		switch(col_dir)
		{
			default:
			case COL_DIR_FORWARD:
			{
				if(col_type == CLIP_WINDOW)
				{
					//Check if we are horizontally lined up to break the window
					//We do this by checking if we are in the area required to start a building to building traversal
					if(is_in_traversal_x_bounds(player->pos))
					{
						current_building->break_window(player->pos,false);
						player->play_sound(snd_winbreak_out,Vec3(0,0,0),0.8f,SOUND_END_TYPE_STOP);
						player_skel->play_anim(PLAYER_ANIM_DEATH_HITWINDOW,ANIM_END_TYPE_FREEZE);
						player_substate_time = t + 1.0f;
						player_substate_time2 = player_substate_time + DEATH_TIME_BLACK_SCREEN_FALLING_FADEIN;
					}
					else
					{
						col_type = CLIP_SOLID;
					}
				}
				if(col_type == CLIP_SOLID)
				{
					player_skel->play_anim(PLAYER_ANIM_DEATH_HITWALL,ANIM_END_TYPE_FREEZE);
					player_substate_time = t + 0.08f;
					player_substate_time2 = player_substate_time + DEATH_TIME_BLACK_SCREEN_FADEIN;
				}
				if(col_type == CLIP_DOORWAY)
				{
					player_skel->play_anim(PLAYER_ANIM_DEATH_HITDOORWAY,ANIM_END_TYPE_FREEZE);
					player_substate_time = t + 0.08f;
					player_substate_time2 = player_substate_time + DEATH_TIME_BLACK_SCREEN_FADEIN;
				}
				if(col_type == CLIP_MID)
				{
					player_skel->play_anim(PLAYER_ANIM_DEATH_HITWAIST,ANIM_END_TYPE_FREEZE);
					player_substate_time = t + 0.15f;
					player_substate_time2 = player_substate_time + DEATH_TIME_BLACK_SCREEN_FADEIN;
				}
				if(col_type == CLIP_SLIDE)
				{
					player_skel->play_anim(PLAYER_ANIM_DEATH_HITSLIDE,ANIM_END_TYPE_FREEZE);
					player_substate_time = t + 0.08f;
					player_substate_time2 = player_substate_time + DEATH_TIME_BLACK_SCREEN_FADEIN;
				}

				break;
			}
			case COL_DIR_LEFT:
			{
				player_skel->play_anim(PLAYER_ANIM_DEATH_HITLEFT,ANIM_END_TYPE_FREEZE);
				player_substate_time = t + 0.08f;
				player_substate_time2 = player_substate_time + DEATH_TIME_BLACK_SCREEN_FADEIN;
				break;
			}
			case COL_DIR_RIGHT:
			{
				player_skel->play_anim(PLAYER_ANIM_DEATH_HITRIGHT,ANIM_END_TYPE_FREEZE);
				player_substate_time = t + 0.08f;
				player_substate_time2 = player_substate_time + DEATH_TIME_BLACK_SCREEN_FADEIN;
				break;
			}
		}
	}
	if(player_sliding)
	{
		switch(col_dir)
		{
			default:
			case COL_DIR_FORWARD:
			{
				if(col_type == CLIP_WINDOW)
				{
					//Check if we are horizontally lined up to break the window
					//We do this by checking if we are in the area required to start a building to building traversal
					if(is_in_traversal_x_bounds(player->pos))
					{
						current_building->break_window(player->pos,false);
						player->play_sound(snd_winbreak_out,Vec3(0,0,0),0.8f,SOUND_END_TYPE_STOP);
						player_skel->play_anim(PLAYER_ANIM_DEATH_SLIDEHITWINDOW,ANIM_END_TYPE_FREEZE);
						player_substate_time = t + 1.0f;
						player_substate_time2 = player_substate_time + DEATH_TIME_BLACK_SCREEN_FALLING_FADEIN;
						break;
					}
				}
				player_skel->play_anim(PLAYER_ANIM_DEATH_SLIDEHITWALL,ANIM_END_TYPE_FREEZE);
				player_substate_time = t + 0.08f;
				player_substate_time2 = player_substate_time + DEATH_TIME_BLACK_SCREEN_FADEIN;
				break;
			}
			case COL_DIR_LEFT:
			{
				player_skel->play_anim(PLAYER_ANIM_DEATH_SLIDEHITLEFT,ANIM_END_TYPE_FREEZE);
				player_substate_time = t + 0.08f;
				player_substate_time2 = player_substate_time + DEATH_TIME_BLACK_SCREEN_FADEIN;
				break;
			}
			case COL_DIR_RIGHT:
			{
				player_skel->play_anim(PLAYER_ANIM_DEATH_SLIDEHITRIGHT,ANIM_END_TYPE_FREEZE);
				player_substate_time = t + 0.08f;
				player_substate_time2 = player_substate_time + DEATH_TIME_BLACK_SCREEN_FADEIN;
				break;
			}
		}

	}
	player_substate = 0.0f;


	if(player_breath_src)
	{
		player_breath_src->stop_audio();
		player_breath_src = NULL;
	}
	if(office_ambience_src)
	{
		office_ambience_src->stop_audio();
		office_ambience_src = NULL;
	}
	if(highrise_ambience_src)
	{
		highrise_ambience_src->stop_audio();
		highrise_ambience_src = NULL;
	}

	player->play_sound(snd_death_impact,Vec3::ZERO(),0.5f,SOUND_END_TYPE_STOP);

	player_state = PLAYER_STATE_DEAD;
	lock_player_rot = true;
}

//returns true if move was successful, returns false if we collided with something and set state to DEAD
bool Game::move_player(Vec3 v)
{
	player_colliding = false;
	//Split up movement into forward, sideways, and vertical movement
	float delta_y = v.y * Time::udelta_time;
	float delta_x = v.x * Time::udelta_time;
	float delta_z = v.z * Time::udelta_time;

	//Checking forward collisions
	Vec3 forward_pos = player->pos + Vec3(0,delta_y,0);

	char clip_dir;
	char clip_type;


	if(clip_player_bbox(forward_pos,clip_dir,clip_type))
	{
		start_player_death(clip_dir,clip_type);
		return false;
	}
	else
	{
		player->pos.y = forward_pos.y;
	}



	//Checking sideways collisions
	Vec3 side_pos = player->pos + Vec3(delta_x,0,0);

	if(clip_player_bbox(side_pos,clip_dir,clip_type))
	{
		start_player_death(clip_dir,clip_type);
		return false;
	}
	else
	{
		player->pos.x = side_pos.x;
	}

	//Don't do collision detection in z-axis (we let PLAYER_STATE_FALLING logic handle that)
	player->pos.z += delta_z;

	return true;
}

void Game::player_run()
{
	if(!player_skel->playing_anim || player_skel->current_anim != 0)
	{
		player_skel->play_anim(PLAYER_ANIM_RUN,ANIM_END_TYPE_LOOP);
	}
	player_head->set_viewbob(CAM_VIEWBOB_RUNNING);
	player_head->update_viewbob();

	if(player->pos.z > current_building->active_floor->altitude)
	{
		player_state = PLAYER_STATE_FALLING;
		return;
	}

	//Make the player move forward, if runs outside of building bounds, reset at building start
	Vec3 movement_vel = Quat(player->angles.y,Vec3::UP()) * Vec3(0,PLAYER_RUN_SPEED,0);

	//Setting player collision precedence array
	cplayer_col_precedence_array = running_col_precedence;

	if(!move_player(movement_vel))
	{
		return;
	}
}


//Executes code if player is at specific frames in specific animations
void Game::player_anim_special_events()
{
	//Checking all frames that the skeleton went through the last update call
	for(int i = 0; i < player_skel->last_frames_passed_count; i++)
	{
		int anim = player_skel->last_frames_passed_anims[i];
		int frame = player_skel->last_frames_passed[i];

		switch(anim)
		{
			default:
				break;
			case PLAYER_ANIM_RUN:
			case PLAYER_ANIM_RUN_PRE_JUMP:
			{
				if(frame == 9)//left foot hit ground
				{
					player->play_sound(footstep_sounds[Random::rand_int_in_range(0,8)],Vec3::ZERO(),0.05f,SOUND_END_TYPE_STOP);
					player_head->viewbob_run_footstep(-viewbob_pitch*DEG_TO_RAD,viewbob_yaw*DEG_TO_RAD,viewbob_roll*DEG_TO_RAD);
				}
				if(frame == 24)//right foot hit ground
				{
					player->play_sound(footstep_sounds[Random::rand_int_in_range(0,8)],Vec3::ZERO(),0.05f,SOUND_END_TYPE_STOP);
					player_head->viewbob_run_footstep(-viewbob_pitch*DEG_TO_RAD,-viewbob_yaw*DEG_TO_RAD,-viewbob_roll*DEG_TO_RAD);
				}
				break;
			}
			case PLAYER_ANIM_SLIDE:
			{
				if(frame == 19)//thigh hits floor
				{
					player->play_sound(snd_slide,Vec3::ZERO(),0.8f,SOUND_END_TYPE_STOP);
				}
				break;
			}
			case PLAYER_ANIM_SLIDE_END:
			{
				if(frame == 7)//right hand hits floor to left
				{
					player->play_sound(snd_hand_contact_low,Vec3(-0.5f,0,0),0.4f,SOUND_END_TYPE_STOP);
				}
				if(frame == 11)//feet step on ground to stand up
				{
					player->play_sound(footstep_sounds[Random::rand_int_in_range(0,8)],Vec3(-0.1f,0,0),0.02f,SOUND_END_TYPE_STOP);
					player->play_sound(footstep_sounds[Random::rand_int_in_range(0,8)],Vec3(0.1f,0,0),0.02f,SOUND_END_TYPE_STOP);
				}
				break;
			}
			case PLAYER_ANIM_DIVE_END:
			{
				if(frame == 16)
				{
					player->play_sound(snd_roll,Vec3(0,0,0),0.7f,SOUND_END_TYPE_STOP);
				}
				break;
			}
			case PLAYER_ANIM_DASH_VAULT:
			{
				if(frame == 1)
				{
					player->play_sound(snd_breath_jump,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				if(frame == 28)//hands smack desk
				{
					player->play_sound(snd_hand_contact_high,Vec3::ZERO(),0.4f,SOUND_END_TYPE_STOP);
				}
				if(frame == 60)
				{
					player->play_sound(snd_land_contact,Vec3(0,0,0),0.7f,SOUND_END_TYPE_STOP);
					player->play_sound(snd_breath_land,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				break;
			}
			case PLAYER_ANIM_KONG:
			{
				if(frame == 1)
				{
					player->play_sound(snd_breath_jump,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				if(frame == 24)//hands smack desk
				{
					player->play_sound(snd_hand_contact_high,Vec3::ZERO(),0.4f,SOUND_END_TYPE_STOP);
				}
				if(frame == 43)
				{
					player->play_sound(snd_land_contact,Vec3(0,0,0),0.3f,SOUND_END_TYPE_STOP);
					player->play_sound(snd_breath_land,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				break;
			}
			case PLAYER_ANIM_SPEED_VAULT:
			{
				if(frame == 1)
				{
					player->play_sound(snd_breath_jump,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				if(frame == 10)//left hand on rail
				{
					player->play_sound(snd_hand_contact_mid,Vec3(-0.5f,0,0),0.4f,SOUND_END_TYPE_STOP);
				}
				if(frame == 12)//right foot on rail
				{
					player->play_sound(footstep_sounds[Random::rand_int_in_range(0,8)],Vec3(1.0f,0,0),0.02f,SOUND_END_TYPE_STOP);
				}
				if(frame == 45)
				{
					player->play_sound(snd_land_contact,Vec3(0,0,0),0.3f,SOUND_END_TYPE_STOP);
					player->play_sound(snd_breath_land,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				break;
			}
			case PLAYER_ANIM_VAULT_SLIDE:
			{
				if(frame == 1)
				{
					player->play_sound(snd_breath_jump,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				if(frame == 14)//left hand on obst
				{
					player->play_sound(snd_hand_contact_mid,Vec3(-0.5f,0,0),0.4f,SOUND_END_TYPE_STOP);
				}
				if(frame == 22)//slide begins
				{
					player->play_sound(snd_hip_contact,Vec3(0,0,0),1.0f,SOUND_END_TYPE_STOP);
					player->play_sound(snd_slide,Vec3(0,0,0),0.8f,SOUND_END_TYPE_STOP);
				}
				if(frame == 64)
				{
					player->play_sound(snd_land_contact,Vec3(0,0,0),0.3f,SOUND_END_TYPE_STOP);
					player->play_sound(snd_breath_land,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				break;
			}
			case PLAYER_ANIM_HIGH_UNDERBAR:
			{
				if(frame == 1)
				{
					player->play_sound(snd_breath_jump,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				if(frame == 17)
				{
					player->play_sound(snd_kick_elevator,Vec3(0,0,0),0.8f,SOUND_END_TYPE_STOP);
				}
				if(frame == 44)
				{
					player->play_sound(snd_hand_metal_contact,Vec3(0,0,0),0.8f,SOUND_END_TYPE_STOP);
				}
				if(frame == 67)
				{
					player->play_sound(snd_body_hit_slide,Vec3(0,0,0),0.8f,SOUND_END_TYPE_STOP);
				}
				if(frame == 113)
				{
					player->play_sound(snd_land_contact,Vec3(0,0,0),0.3f,SOUND_END_TYPE_STOP);
					player->play_sound(snd_breath_land,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				if(frame == 127)
				{
					player->play_sound(snd_roll,Vec3(0,0,0),0.3f,SOUND_END_TYPE_STOP);
				}
				break;
			}
			case PLAYER_ANIM_UNDERBAR:
			{
				if(frame == 1)
				{
					player->play_sound(snd_breath_jump,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				if(frame == 11)//hands contact wood
				{
					player->play_sound(snd_hand_contact_mid,Vec3(0,0,0),0.4f,SOUND_END_TYPE_STOP);
				}
				if(frame == 32)
				{
					player->play_sound(snd_land_contact,Vec3(0,0,0),0.7f,SOUND_END_TYPE_STOP);
					player->play_sound(snd_breath_land,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				break;
			}
			case PLAYER_ANIM_TRAV_A:
			{
				if(frame == 1)
				{
					player->play_sound(snd_breath_jump,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				if(frame == 140)
				{
					player->play_sound(snd_land_contact,Vec3(0,0,0),0.7f,SOUND_END_TYPE_STOP);
					player->play_sound(snd_breath_land,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				if(frame == 145)
				{
					player->play_sound(snd_roll,Vec3(0,0,0),0.7f,SOUND_END_TYPE_STOP);
				}
				break;
			}
			case PLAYER_ANIM_TRAV_B:
			case PLAYER_ANIM_TRAV_C:
			{
				if(frame == 1)
				{
					player->play_sound(snd_breath_jump,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				if(frame == 139)
				{
					player->play_sound(snd_land_contact,Vec3(0,0,0),0.7f,SOUND_END_TYPE_STOP);
					player->play_sound(snd_breath_land,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				}
				if(frame == 143)
				{
					player->play_sound(snd_roll,Vec3(0,0,0),0.7f,SOUND_END_TYPE_STOP);
				}
				break;
			}
		}
	}
}

void Game::player_noclip_logic()
{
	//Touch input zones:
	//look left/right/up/down
	//move left/right/forward/back
	//move up/down

	//Camera velocity
	float cam_vel = 60.0f;

	//Camera angular velocity
	float cam_ang_vel = 140.0f * DEG_TO_RAD;

	Entity* move_ent = NULL;

	if(player_state == PLAYER_STATE_NOCLIP)
		move_ent = player;
	else
		move_ent = camera;

	//Checking input from all fingers:
	for(int i = 0; i < MAX_INPUT_TOUCHES; i++)
	{
		if(!(input_touching[i]))
			continue;
		float x = input_x[i];
		float y = input_y[i];

		float sx = input_start_x[i];
		float sy = input_start_y[i];

		//top right corner: go back to player run mode
		if(sx >= 0.66f)
		{
			if(sy >= 0.85f)
			{
				input_touching[i] = false;
				if(player_state == PLAYER_STATE_NOCLIP)
				{
					player_state = PLAYER_STATE_CAM_FLY;
					player->angles = Vec3::ZERO();
					if(player->pos.z <= current_building->active_floor->altitude)
					{
						player->pos.z = current_building->active_floor->altitude+0.01f;
					}
					if(player->pos.x >= current_building->active_floor->global_maxs.x - 0.5f)
					{
						player->pos.x = current_building->active_floor->global_maxs.x - 1.0f;
					}
					if(player->pos.x <= current_building->active_floor->global_mins.x + 0.5f)
					{
						player->pos.x = current_building->active_floor->global_mins.x + 1.0f;
					}

					if(player->pos.y <= current_building->active_floor->global_mins.y + 0.5f)
					{
						player->pos.y = current_building->active_floor->global_mins.y + 1.0f;
					}
					return;
				}
				else
				{
					player_state = PLAYER_STATE_RUNNING;
					return;
				}
			}
		}
		//top left corner
		if(sx <= 0.33f)
		{
			if(sy >= 0.85f)
			{
				if(player_state == PLAYER_STATE_NOCLIP)
				{
					input_touching[i] = false;
				}
				//Reset camera offset
				if(player_state == PLAYER_STATE_CAM_FLY)
				{
					camera->pos = Vec3::ZERO();
					camera->angles = Vec3::ZERO();
				}
			}
		}

		//bottom third right half is camera view direction
		if(sx > 0.5f)
		{
			if(sy < 0.33f)
			{
				//float delta_y = fmaxf(fminf((y - 0.165f) / 0.17f,1.0f),-1.0f);//measured from area center
				float delta_y = fmaxf(fminf((y - sy) / 0.17f,1.0f),-1.0f);//measured from touch start pos
				//float delta_x = -1.0f * fmaxf(fminf((x - 0.75f) / 0.25f,1.0f),-1.0f);//measured from area center
				float delta_x = -1.0f * fmaxf(fminf((x - sx) / 0.25f,1.0f),-1.0f);//measured from touch start pos
				//Making the controls be cubic in acceleration
				delta_x *= delta_x * delta_x;
				delta_y *= delta_y * delta_y;
				delta_x *= cam_ang_vel;
				delta_y *= cam_ang_vel;
				move_ent->angles.x += delta_y * Time::udelta_time;
				move_ent->angles.y += delta_x * Time::udelta_time;
				continue;
			}
		}
		//bottom third left half is camera movement
		if(sx < 0.5f)
		{
			if(sy < 0.33f)
			{
				//float delta_y = fmaxf(fminf((y - 0.165f) / 0.17f,1.0f),-1.0f);//measured from area center
				float delta_y = fmaxf(fminf((y - sy) / 0.17f,1.0f),-1.0f);//measured from touch start pos
				//float delta_x = fmaxf(fminf((x - 0.25f) / 0.25f,1.0f),-1.0f);//measured from area center
				float delta_x = fmaxf(fminf((x - sx) / 0.25f,1.0f),-1.0f);//measured from touch start pos
				//Making movement be cubic
				delta_x *= delta_x * delta_x;
				delta_y *= delta_y * delta_y;
				delta_x *= cam_vel;
				delta_y *= cam_vel;
				Vec3 forward, right, up;
				move_ent->angles.angles_to_dirs(&forward,&right,&up);
				move_ent->pos = move_ent->pos + (forward * delta_y * Time::udelta_time) + (right * delta_x * Time::udelta_time);
				continue;
			}
		}

		//second third left half is camera height
		if(sx < 0.5f)
		{
			if(sy >= 0.33f && sy < 0.66f)
			{
				//float delta_y = fminf((y - 0.5f) / 0.17f,1.0f);//measured from area center
				float delta_y = fminf((y - sy) / 0.17f,1.0f);//measured from touch start pos
				//Making movement be cubic
				delta_y *= delta_y * delta_y;
				delta_y *= cam_vel;
				Vec3 forward, right, up;
				move_ent->angles.angles_to_dirs(&forward,&right,&up);
				move_ent->pos = move_ent->pos + (up * delta_y * Time::udelta_time);
				continue;
			}
		}

		//For floor generation
		if(player_state == PLAYER_STATE_CAM_FLY)
		{
			if(sx < 0.5f)
			{
				if(sy >= 0.66f && sy < 0.85f)
				{
					input_touching[i] = false;
					current_building->regenerate_floor(player->pos,buildings[NEXT_BLDG[cbldg_index]]);
					continue;
				}
			}
		}
	}
}

void Game::player_state_logic()
{
	float t = Time::time();

	if(player_state == PLAYER_STATE_TRAVERSING)
	{
		player_head->set_viewbob(mnvr_frame->viewbob_type);
		player_head->update_viewbob();
		mnvr_movement();
		return;
	}

	if(player_state == PLAYER_STATE_MANEUVERING)
	{
		player_head->set_viewbob(mnvr_frame->viewbob_type);
		player_head->update_viewbob();
		mnvr_movement();
		return;
	}

	if(player_state == PLAYER_STATE_RUNNING)
	{
		//Checking for traversals

		Vec3 temp_tile_ofs = Vec3::ZERO();

		Traversal* trav = current_building->input_to_traversal(player->pos, INPUT_SWIPE_NONE | input_swipe,temp_tile_ofs);

		if(trav)
		{
			player_state = PLAYER_STATE_TRAVERSING;
			trav_current = trav;
			mnvr_current = trav;
			mnvr_tile_ofs = temp_tile_ofs;
			mnvr_frame_number = -1;
			mnvr_next_frame_number = 0;
			mnvr_next_frame = trav->keyframes[0];
			reached_mnvr_keyframe();
			return;
		}

		//Check for maneuvers that require no input or whatever input we have sent (input_swipe)
		Maneuver* man = current_building->input_to_maneuver(player->pos, INPUT_SWIPE_NONE | input_swipe,temp_tile_ofs);

		if(man)
		{
			player_state = PLAYER_STATE_MANEUVERING;
			mnvr_current = man;
			mnvr_tile_ofs = temp_tile_ofs;
			mnvr_frame_number = -1;
			mnvr_next_frame_number = 0;
			mnvr_next_frame = man->keyframes[0];
			reached_mnvr_keyframe();
			return;
		}
		else if(input_swipe)
		{
			//There was a swipe, but we are not in a maneuver area:
			if(input_swipe == INPUT_SWIPE_UP)
			{
				player_state = PLAYER_STATE_FALLING;
				player_phys_vel.z = PLAYER_JUMP_VEL;
				player_phys_vel = player_phys_vel + (Quat(player->angles.y,Vec3::UP()) * Vec3(0,PLAYER_RUN_SPEED,0));
				player->play_sound(snd_breath_jump,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
				player_skel->play_anim(PLAYER_ANIM_RUN_JUMP,ANIM_END_TYPE_DEFAULT_ANIM);
				input_swipe = INPUT_SWIPE_NONE;
				if(player_breath_src)
				{
					player_breath_src->stop_audio();
					player_breath_src = NULL;
				}
				return;
			}
			//Get maneuvers that require input down
			if(input_swipe == INPUT_SWIPE_DOWN)
			{
				player_state = PLAYER_STATE_SLIDING;
				player_slide_speed = PLAYER_SLIDE_SPEED;
				player_substate = 0.0f;
				//In this context, subtate_time is the time that the player will slide for
				player_substate_time = t + PLAYER_SLIDE_TIME;
				player_skel->play_anim(PLAYER_ANIM_SLIDE,ANIM_END_TYPE_FREEZE);
				input_swipe = INPUT_SWIPE_NONE;
				return;
			}
		}

		player_run();

		//Then check for more general out-of-boundsness in both x and y axes
		//player->pos = player->pos + Time::delta_time * movement_vel;
		if(current_building->is_box_out_of_bounds(player->pos,PLAYER_SIZE))
		{
			player->pos = current_building->active_floor->global_pos + Vec3(0.0f,1.0f,0.0f);
		}

	}
	if(player_state == PLAYER_STATE_FALLING)
	{
		player_phys_vel.z += -9.8 * Time::udelta_time;
		if(player_phys_vel.z < -40.0f)//terminal vel
			player_phys_vel.z = -40.0f;

		Vec3 delta_pos = Time::udelta_time * player_phys_vel;

		if(player->pos.z + delta_pos.z <= current_building->active_floor->altitude)
		{
			player->play_sound(snd_breath_land,Vec3(0,0,0),0.6f,SOUND_END_TYPE_STOP);
			player_state = PLAYER_STATE_RUNNING;
			player->pos.z = current_building->active_floor->altitude;
			player_phys_vel = Vec3::ZERO();
			return;
		}

		//Setting player collision precedence array
		//If the player is 0.35m above the floor (doorways are 2.35 m high, player is 2 m tall)
		//We use jumping col precedence array, else we use the running col precedence array
		if(player->pos.z + delta_pos.z >= current_building->active_floor->altitude + 0.35f)
		{
			cplayer_col_precedence_array = jumping_col_precedence;
		}
		else
		{
			cplayer_col_precedence_array = running_col_precedence;
		}

		if(!move_player(player_phys_vel))
		{
			return;
		}
	}
	if(player_state == PLAYER_STATE_SLIDING)
	{
		//substate 0 = player is transitioning into the slide
		//subtate 1 = player is on the ground sliding
		if(player_substate_time < t)
		{
			if(player_substate == 0.0f)
			{
				player_substate = 1.0f;
				player_substate_time = t + 0.383f;//animation is 23 frames long, 23 frames @ 60 fps = 0.383 seconds
				player_skel->play_anim(PLAYER_ANIM_SLIDE_END,ANIM_END_TYPE_FREEZE);
			}
			else if(player_substate == 1.0f)
			{
				player_state = PLAYER_STATE_RUNNING;
				player_slide_speed = 0.0f;
				player_substate = 0.0f;
				player_substate_time = 0.0f;
				player_skel->play_default_anim();
			}
		}
		//In this context, substate_time is the time that the player will slide for

		player_slide_speed += PLAYER_SLIDE_ACCEL * Time::udelta_time;
		if(player_slide_speed < PLAYER_SLIDE_MIN_SPEED)
			player_slide_speed = PLAYER_SLIDE_MIN_SPEED;

		player_phys_vel = (Quat(player->angles.y,Vec3::UP()) * Vec3(0,player_slide_speed,0));
		player_head->set_viewbob(CAM_VIEWBOB_SLIDING);
		player_head->update_viewbob();

		//Setting player collision precedence array
		cplayer_col_precedence_array = sliding_col_precedence;

		if(!move_player(player_phys_vel))
		{
			return;
		}
	}
	if(player_state == PLAYER_STATE_DEAD)
	{
		//Substate 0: black screen fading in
		//Substate 1: white screen fading in
		//Substate 2: white screen fading out

		//Black Screen fading in
		if(player_substate == 0.0f)
		{
			if(t > player_substate_time)
			{
				black_overlay_opacity = fminf((t - player_substate_time) / (player_substate_time2 - player_substate_time),1.0f);
				if(t > player_substate_time2 + DEATH_TIME_BLACK_SCREEN)
				{
					player_substate_time = t;
					player_substate_time2 = t + DEATH_TIME_WHITE_SCREEN_FADEIN;
					player_substate = 1.0f;
					player->play_sound(snd_death_trans,Vec3::ZERO(),0.8f,SOUND_END_TYPE_STOP);
				}
			}
		}
		//White screen fading in
		if(player_substate == 1.0f)
		{
			if(t > player_substate_time)
			{
				white_overlay_opacity = fminf((t - player_substate_time) / (player_substate_time2 - player_substate_time),1.0f);
				if(t > player_substate_time2 + DEATH_TIME_WHITE_SCREEN)
				{
					player_substate_time = t;
					player_substate_time2 = t + DEATH_TIME_WHITE_SCREEN_FADEOUT;
					player_substate = 2.0f;
					//Resetting things here
					reset();
					player_skel->play_anim(PLAYER_ANIM_RUN,ANIM_END_TYPE_LOOP);
					//Make sure the player stays dead
					player_state = PLAYER_STATE_DEAD;
					lock_player_rot = false;
				}
			}
		}
		//White screen fading out
		if(player_substate == 2.0f)
		{
			if(t > player_substate_time)
			{
				//Execute running logic
				//player_state = PLAYER_STATE_RUNNING;
				//player_run();
				//player_state = PLAYER_STATE_DEAD;

				white_overlay_opacity = fmaxf((player_substate_time2 - t) / (player_substate_time2 - player_substate_time),0.0f);
				black_overlay_opacity = white_overlay_opacity;

				if(t > player_substate_time2)
				{
					player_state = PLAYER_STATE_RUNNING;
					player_substate = 0.0f;
					player_substate_time = 0.0f;
					player_substate_time2 = 0.0f;
				}
			}
		}
	}
}

void Game::update()
{
	//====Test show/hide ad calls====
	static bool ad_visible = false;
	if(input_x[4] < 0.10f && input_touching[4] && ad_visible)
	{
		//jnii->hide_ad();
		jnii->hide_keyboard();
		ad_visible = false;
	}
	else if(input_x[4] > 0.90f && input_touching[4] && !ad_visible)
	{
		//jnii->show_ad();
		jnii->show_keyboard();
		ad_visible = true;
	}
	//============================


	float t = Time::time();


	for(int i = 0; i < MAX_BUILDINGS; i++)
	{
		buildings[i]->update();
	}


	//Handling and moving ambient audio sources
	if(current_building->is_in_bounds_or_near_front(player->pos) && office_ambience_src)
	{
		office_ambience_src->pos = current_building->clamp_to_bounds(player->pos);
	}
	if(current_building->is_out_of_bounds(player->pos) && highrise_ambience_src)
	{
		highrise_ambience_src->pos = player->pos;
	}

	//Handling breath logic
	if(player_breath_src)
	{
		if(player_state != PLAYER_STATE_RUNNING && player_state != PLAYER_STATE_SLIDING && player_state != PLAYER_STATE_NOCLIP && player_state != PLAYER_STATE_CAM_FLY)
		{
			player_breath_src->stop_audio();
			player_breath_src = NULL;
		}
	}
	else if(!player_breath_src)
	{
		if(player_state == PLAYER_STATE_RUNNING || player_state == PLAYER_STATE_SLIDING)
		{
			player_breath_src = player->play_sound(snd_breath,Vec3(0,0,0),0.6f,SOUND_END_TYPE_LOOP);
		}
	}

	if(player_state == PLAYER_STATE_NOCLIP || player_state == PLAYER_STATE_CAM_FLY)
	{
		player_noclip_logic();
		return;
	}

	//TODO: handle input here, remove this
	//Checking touch button interactions
	//Checking input from all fingers:
	for(int i = 0; i < MAX_INPUT_TOUCHES; i++)
	{
		if(!(input_touching[i]))
			continue;
		float x = input_start_x[i];
		float y = input_start_y[i];

		//top right corner: go to player noclip mode
		if(x >= 0.66f && y >= 0.85f)
		{
			player_state = PLAYER_STATE_NOCLIP;
			black_overlay_opacity = 0.0f;
			white_overlay_opacity = 0.0f;
			input_touching[i] = false;
			player->angles.x = 0.0f;
			player->angles.y = 0.0f;
			player->angles.z = 0.0f;
			player_head->viewbob_angles = Vec3::ZERO();
			player_head->viewbob_vel = Vec3::ZERO();
			return;
		}
		/*//For toggling viewbob edit menu
		if(x <= 0.33f && y >= 0.85f)
		{
			input_touching[i] = false;
			viewbob_menu_state++;
			if(viewbob_menu_state >= 3)
				viewbob_menu_state = 0;
			continue;
		}
		//For zeroing out the viewbob
		if(x > 0.33f && x < 0.66f && y >= 0.85f)
		{
			input_touching[i] = false;

			camera->viewbob_vel = Vec3::ZERO();
			camera->angles = Vec3::ZERO();
		}

		//left 10% = 0, right 10% = 1.0
		float clipped_x = fmaxf(fminf((x-0.1f)/0.8f,1.0f),0.0f);
		//Altering run viewbob values
		if(viewbob_menu_state == 1)
		{
			//range is [0-100]
			//Edit top field
			if(y > 0.72 && y < 0.80)
			{
				viewbob_pitch = 100 * clipped_x;
				continue;
			}
			//Edit middle field
			if(y > 0.64 && y < 0.72)
			{
				viewbob_yaw = 100 * clipped_x;
				continue;
			}
			//Edit bottom field
			if(y > 0.56 && y < 0.64)
			{
				viewbob_roll = 100 * clipped_x;
				continue;
			}
		}
		//Altering camera parameters
		if(viewbob_menu_state == 2)
		{
			//Edit top field
			if(y > 0.72 && y < 0.80)
			{
				//due to massive range, I want this to be on a cubic scale
				camera->viewbob_spring_constant = 10000 * clipped_x * clipped_x * clipped_x;
				continue;
			}
			//Edit middle field
			if(y > 0.64 && y < 0.72)
			{
				//I want this to be on a cubic scale as well
				camera->viewbob_resistance = 10 * clipped_x * clipped_x * clipped_x;
				continue;
			}
			//Edit bottom field
			if(y > 0.56 && y < 0.64)
			{
				camera->viewbob_max_stray = 90 * clipped_x;
				continue;
			}
		}*/
	}

	//If modifying viewbob, run forward without checking collisions
	/*if(viewbob_menu_state != 0)
	{
		player->pos.y += Time::udelta_time * PLAYER_RUN_SPEED;

		if(!player_skel->playing_anim || player_skel->current_anim != 0)
		{
			player_skel->play_anim(PLAYER_ANIM_RUN,ANIM_END_TYPE_LOOP);
		}
		player_anim_special_events();
		camera->set_viewbob(CAM_VIEWBOB_RUNNING);
		camera->update_viewbob();
		player->update();

		if(player->pos.y > current_building->global_maxs.y)
		{
			player->pos.y = current_building->global_mins.y + 1.0f;
		}
		return;
	}*/

	camera_roll_tilt_angle = 0.0f;
	player_goal_yaw = 0.0f;
	//Player turning code:
	if(input_turning && !lock_player_rot && (player_state == PLAYER_STATE_RUNNING || player_state == PLAYER_STATE_SLIDING))
	{
		player_goal_yaw = -PLAYER_MAX_TURN_ANGLE * input_turn * DEG_TO_RAD;
	}

	//Slight camera roll rotation when turning
	if(player_state != PLAYER_STATE_MANEUVERING && player_state != PLAYER_STATE_TRAVERSING)
	{
		camera_roll_tilt_angle = (player->angles.y - player_goal_yaw) * 0.8f;
		camera_roll_tilt_angle = efmodf(camera_roll_tilt_angle + PI,TWO_PI) - PI;
	}
	player_head->tilt_angles.z = lerp_wtd_avg(player_head->tilt_angles.z,camera_roll_tilt_angle,5.0f);

	player->angles.y += (player_goal_yaw - player->angles.y) * PLAYER_TURN_LERP_FACTOR;
	player->update();
	player_anim_special_events();
	player_state_logic();
}

//Draws the scene
void Game::render()
{
	//Setting all transforms to be recalculated
	player_skel->transform_calculated = false;
	camera->transform_calculated = false;
	player_head->transform_calculated = false;
	player->transform_calculated = false;
	cam_to_bone->transform_calculated = false;

	for(int i = 0; i < MAX_BUILDINGS; i++)
	{
		buildings[i]->transform_calculated = false;
		buildings[i]->broken_owindow_skel->transform_calculated = false;
		buildings[i]->broken_iwindow_skel->transform_calculated = false;
	}

	//glClear(GL_COLOR_BUFFER_BIT);

	camera->update_view_matrix();

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Setting up global shader parameters
	//Time
	float t = Time::time();
	Shader::set_static_global_param(Shader::GLOBAL_PARAM_FLOAT_TIME,&t);
	//Camera direction
	float cam_dir[] = {camera->forward.x,camera->forward.y,camera->forward.z};
	Shader::set_static_global_param(Shader::GLOBAL_PARAM_VEC3_CAM_DIR, cam_dir);
	//Camera position
	//Evaluating global camera position
	Vec3 global_cam_pos = camera->world_transform.get_pos();
	float cam_pos[] = {global_cam_pos.x,global_cam_pos.y,global_cam_pos.z};
	Shader::set_static_global_param(Shader::GLOBAL_PARAM_VEC3_CAM_POS, cam_pos);
	//Directional light direction
	float light_dir[] = {0,0,-1};
	Shader::set_static_global_param(Shader::GLOBAL_PARAM_VEC3_DIRLIGHT_DIR, light_dir);
	//Directional light color
	float light_col[] = {1.0,0.0,0.0};
	Shader::set_static_global_param(Shader::GLOBAL_PARAM_VEC3_DIRLIGHT_COL, light_col);

	Mat4 vp = camera->persp_proj_m * camera->view_m;

	player->render(vp);
	Mat4 view_no_translation = camera->inf_proj_m * ((camera->view_m).pos_removed());


	//buildings[0]->render(player->pos,vp);
	for(int i = 0; i < MAX_BUILDINGS; i++)
	{
		buildings[i]->render(player->pos,vp);
	}

	skybox->render(view_no_translation);
	//Rendering transparent objects after skybox

	//buildings[0]->render_transparent_meshes(player->pos,vp);
	for(int i = 0; i < MAX_BUILDINGS; i++)
	{
		buildings[i]->render_transparent_meshes(player->pos,vp);
	}

	//draw_floor_collision_voxels(vp);
	//draw_floor_maneuvers(vp);
	//if(player_state != PLAYER_STATE_MANEUVERING && player_state != PLAYER_STATE_TRAVERSING)
	//	draw_player_bbox(vp);

	//=============================== Screen UI rendering begins here ===============================
	render_screen_overlay();

	/*if(player_state != PLAYER_STATE_NOCLIP && player_state != PLAYER_STATE_CAM_FLY)
	{
		//"Edit	Reset Vbob"
		if(viewbob_menu_state == 0)
		{
			char test_viewbob_str[40];
			snprintf(test_viewbob_str,40,"[Edit]\t[Zero]");
			test_text->set_text(test_viewbob_str);
		}
		if(viewbob_menu_state == 1)
		{
			char test_viewbob_str[120];
			snprintf(test_viewbob_str,120,"[Edit]\t[Zero]\n\nEdit run velocities\nPitch\n\t%.2f\nYaw\n\t%.2f\nRoll\n\t%.2f",viewbob_pitch,viewbob_yaw,viewbob_roll);
			test_text->set_text(test_viewbob_str);
		}
		if(viewbob_menu_state == 2)
		{
			char test_viewbob_str[120];
			snprintf(test_viewbob_str,120,"[Edit]\t[Zero]\n\nEdit cam properties\nSpring Constant\n\t%.2f\nResistance\n\t%.2f\nMax Stray\n\t%.2f",camera->viewbob_spring_constant,camera->viewbob_resistance,camera->viewbob_max_stray);
			test_text->set_text(test_viewbob_str);
		}
		//char test_viewbob_str[40];
		//snprintf(test_viewbob_str,40,"t=%f",t);
		//test_text->set_text(test_viewbob_str);
		test_text->render(camera->ortho_proj_m);
	}*/

	if(player_state == PLAYER_STATE_NOCLIP)
	{
		//UI_Text::draw_text("Mode:\n NOCLIP", Vec3(-screen_width * 0.4f,screen_height * 0.45f,0.5f), Vec3(0,0,0), 100.0f, Vec3(1,1,1), Vec3(0,0,0), 1.0f, false, camera->ortho_proj_m);
	}
	if(player_state == PLAYER_STATE_CAM_FLY)
	{
		//UI_Text::draw_text("Mode:\n CAM FLY", Vec3(-screen_width * 0.4f,screen_height * 0.45f,0.5f), Vec3(0,0,0), 100.0f, Vec3(1,1,1), Vec3(0,0,0), 1.0f, false, camera->ortho_proj_m);
	}

}