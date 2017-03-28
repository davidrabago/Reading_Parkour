//
// Created by F1 on 8/22/2016.
//

#include "Building.hpp"


//Dispatches to floor's input_to_maneuver
//	ultimately returns a maneuver if there exists a maneuver in the current floor's tileset such that:
//	(the input required to start the maneuver is input_type) AND (the player is within the bounding box required to start the maneuver)
//	returns NULL if no such Maneuver exists
Maneuver* Building::input_to_maneuver(Vec3 pos, int input_type, Vec3& tile_ofs)
{
	return active_floor->input_to_maneuver(pos,input_type, tile_ofs);
}

//Dispatches to floor's input_to_traversal
//	ultimately returns a traversal if we are at the last tile of the building and:
//	(input_type is the input required to start the traversal) AND (the player is within the bounding box required to start the traversal)
//	returns NULL if no such traversal exists
Traversal* Building::input_to_traversal(Vec3 pos, int input_type, Vec3& tile_ofs)
{
	return active_floor->input_to_traversal(pos,input_type,tile_ofs);
}


Vec3 Building::get_tile_ofs_at_pos(Vec3 p)
{
	return active_floor->get_tile_ofs_at_pos(p);
}


Building::Building()
{
	active_floor = new Floor();
	clear();
}
Building::~Building()
{
	if(active_floor)
		delete active_floor;
}

//Creates the building
void Building::generate(Building* prev_bldg,Vec3 bldg_ofs)
{
	if(generated)
		return;
	floors = 20;

	dimensions = Vec3(0,0,floors);
	dimensions.x = Random::rand_int_in_range(BUILDING_MIN_WIDTH,BUILDING_MAX_WIDTH);
	dimensions.y = Random::rand_int_in_range(BUILDING_MIN_LENGTH,BUILDING_MAX_LENGTH);

	size = Vec3(dimensions.x * TILE_SIZE, dimensions.y * TILE_SIZE, dimensions.z * WINDOW_TILE_SIZE);


	Vec3 prev_bldg_ofs = Vec3(0,0,0);
	int lateral_ofs = 0;
	if(prev_bldg != NULL)
	{
		prev_bldg_ofs = Vec3(prev_bldg->pos.x,prev_bldg->global_maxs.y,0);

		//Can only offset to the left by half of our width
		int ofs_min = ((int)(-dimensions.x / 2));
		//Can only offset to the right by half of the last building's width
		int ofs_max = (int) (prev_bldg->dimensions.x / 2);

		lateral_ofs = Random::rand_int_in_range(ofs_min, ofs_max + 1);

		//LOGI("Previous building pos:(%.2f,%.2f,%.2f), dims:(%.1f,%.1f,%1.f)",prev_bldg->pos.x,prev_bldg->pos.y,prev_bldg->pos.z,prev_bldg->dimensions.x,prev_bldg->dimensions.y,prev_bldg->dimensions.z);
		//LOGI("Current building dims:(%.1f,%.1f,%1.f)",dimensions.x,dimensions.y,dimensions.z);
		//LOGI("Lateral ofs range: [%d,%d]: res = %d",ofs_min,ofs_max,lateral_ofs);

	}
	pos = prev_bldg_ofs + bldg_ofs + Vec3(lateral_ofs*TILE_SIZE,0,BUILDING_GROUNDLEVEL);

	global_mins = Vec3(pos.x, pos.y, pos.z);
	global_maxs = global_mins + size;

	generate_exterior_model_list();
	generated = true;
}

void Building::generate_floor(Vec3 player_pos, Building* next_bldg)
{
	if(!generated)
		return;

	//Finding which floor number the player is in:
	active_floor_number = (int) (((player_pos.z - BUILDING_GROUNDLEVEL))/WINDOW_TILE_SIZE);

	//Finding our goal column range
	//(what tiles of this building line up with tiles of next_bldg)
	//Finding tile dimensions of next_bldg in terms of this building's tile dimensions
	int other_left_endpnt = (int)((next_bldg->pos.x - pos.x)/TILE_SIZE);
	int other_right_endpnt = ((int)next_bldg->dimensions.x) + other_left_endpnt;

	int goal_min = clamp(0,other_left_endpnt,other_right_endpnt-1);
	int goal_max = clamp((int)dimensions.x-1,other_left_endpnt,other_right_endpnt-1);

	active_floor->generate(pos,active_floor_number,global_mins,global_maxs,dimensions,player_pos,goal_min,goal_max);
	generate_interior_model_list();
}

//Currently regenerates the building using different rng numbers
void Building::regenerate_floor(Vec3 player_pos, Building* next_bldg)
{
	int temp = active_floor_number;

	//We cannot change the size of the building, so back up the dimensions and pos
	Vec3 old_pos = pos;
	Vec3 old_dims = dimensions;

	clear();
	active_floor_number = temp;
	active_floor->clear();

	pos = old_pos;
	dimensions = old_dims;

	floors = (int)old_dims.z;

	size = Vec3(dimensions.x * TILE_SIZE, dimensions.y * TILE_SIZE, dimensions.z * WINDOW_TILE_SIZE);

	global_mins = pos;
	global_maxs = pos + size;

	generate_exterior_model_list();
	generated = true;
	generate_interior_model_list();

	//Finding our goal column range
	//(what tiles of this building line up with tiles of next_bldg)
	//Finding tile dimensions of next_bldg in terms of this building's tile dimensions
	int other_left_endpnt = (int)((next_bldg->pos.x - pos.x)/TILE_SIZE);
	int other_right_endpnt = ((int)next_bldg->dimensions.x) + other_left_endpnt;

	int goal_min = clamp(0,other_left_endpnt,other_right_endpnt-1);
	int goal_max = clamp((int)dimensions.x-1,other_left_endpnt,other_right_endpnt-1);
	//LOGI("This Building: (x-pos:%.2f, x-dim:%.1f), Next Building: (x-pos:%.2f, x-dim:%.1f), goal range:[%d,%d]",pos.x,dimensions.x,next_bldg->pos.x,next_bldg->dimensions.x,goal_min,goal_max);

	active_floor->generate(pos,active_floor_number,global_mins,global_maxs,dimensions,player_pos,goal_min,goal_max);
}

//Clears the created building, zeroes everything out
void Building::clear()
{
	active_floor_number = 0;
	floors = 0;
	dimensions = Vec3(0,0,0);
	size = Vec3(0,0,0);
	pos = Vec3(0,0,0);
	global_mins = Vec3(0,0,0);
	global_maxs = Vec3(0,0,0);

	broken_iwindow_index_x = -1;
	broken_iwindow_index_y = -1;
	broken_owindow_index_x = -1;
	broken_owindow_index_y = -1;

	broken_iwindow_active = false;
	broken_owindow_active = false;
	if(broken_iwindow_skel)
		broken_iwindow_skel->stop_anim();
	if(broken_owindow_skel)
		broken_owindow_skel->stop_anim();

	ext_mdl_fw_count = 0;
	ext_mdl_bw_count = 0;
	ext_mdl_lw_count = 0;
	ext_mdl_rw_count = 0;

	int_mdl_fw_count = 0;
	int_mdl_bw_count = 0;
	int_mdl_lw_count = 0;
	int_mdl_rw_count = 0;

	active_floor->clear();
	generated = false;
}

//Returns the floor's voxel at point p
Voxel Building::get_voxel_at(Vec3 p)
{
	return active_floor->get_voxel_at(p);
}
//Returns if the floor has a voxel that is solid at the point p
char Building::is_solid_at(Vec3 p)
{
	return active_floor->is_solid_at(p);
}

//Returns true if the point p is out of the building's inner bounds, false if not
bool Building::is_out_of_bounds(Vec3 p)
{
	if(p.x < global_mins.x || p.x > global_maxs.x)
		return true;
	if(p.y < global_mins.y || p.y > global_maxs.y)
		return true;
	//Z bounds are not handled here
	return false;
}

//Returns true if the point p is in the building, or within 5 m of the front of the building
bool Building::is_in_bounds_or_near_front(Vec3 p)
{
	if(p.x < global_mins.x || p.x > global_maxs.x)
		return false;
	if(p.y < global_mins.y - 5.0f || p.y > global_maxs.y)
		return false;
	//Z bounds are not handled here
	return true;
}

//Returns true if any corner of the bounding box 2*half_width x 2*half_width located at point p is out of bounds
bool Building::is_box_out_of_bounds(Vec3 p, float half_width)
{
	if(p.x-half_width < global_mins.x || p.x+half_width > global_maxs.x)
		return true;
	if(p.y-half_width < global_mins.y || p.y+half_width > global_maxs.y)
		return true;
	//Z bounds are not handled here
	return false;
}

//Returns Vec3 in bounds
Vec3 Building::clamp_to_bounds(Vec3 p)
{
	return Vec3(clampf(p.x,global_mins.x,global_maxs.x),clampf(p.y,global_mins.y,global_maxs.y),clampf(p.z,global_mins.z,global_maxs.z));
}


//Recursively 2d partitions a wall into the largest (nxn) (n being 2^i for all integers i in [0,5]) tiles that fit
//Then we draw these tiles, drastically reducing the draw call counts for the buildings
void Building::subdivide_wall(Mat4 trans, int wall_width, int wall_height, int* mdl_count, Static_Model **mdl_list, Mat4* trans_list)
{
	Static_Model* models[6];
	models[0] = Global_Tiles::instance->window_styles[0]->window_models->tile_model;
	models[1] = Global_Tiles::instance->window_styles[0]->window_models->m2x2_model;
	models[2] = Global_Tiles::instance->window_styles[0]->window_models->m4x4_model;
	models[3] = Global_Tiles::instance->window_styles[0]->window_models->m8x8_model;
	models[4] = Global_Tiles::instance->window_styles[0]->window_models->m16x16_model;
	models[5] = Global_Tiles::instance->window_styles[0]->window_models->m32x32_model;

	Static_Model* hor_models[6];
	hor_models[0] = Global_Tiles::instance->window_styles[0]->window_models->tile_model;
	hor_models[1] = Global_Tiles::instance->window_styles[0]->window_models->m1x2_model;
	hor_models[2] = Global_Tiles::instance->window_styles[0]->window_models->m1x4_model;
	hor_models[3] = Global_Tiles::instance->window_styles[0]->window_models->m1x8_model;
	hor_models[4] = Global_Tiles::instance->window_styles[0]->window_models->m1x16_model;
	hor_models[5] = Global_Tiles::instance->window_styles[0]->window_models->m1x32_model;

	Static_Model* vert_models[6];
	vert_models[0] = Global_Tiles::instance->window_styles[0]->window_models->tile_model;
	vert_models[1] = Global_Tiles::instance->window_styles[0]->window_models->m2x1_model;
	vert_models[2] = Global_Tiles::instance->window_styles[0]->window_models->m4x1_model;
	vert_models[3] = Global_Tiles::instance->window_styles[0]->window_models->m8x1_model;
	vert_models[4] = Global_Tiles::instance->window_styles[0]->window_models->m16x1_model;
	vert_models[5] = Global_Tiles::instance->window_styles[0]->window_models->m32x1_model;

	for(int i = BUILDING_WINDOW_MAX_TILE_MATRIX; i >= 0; i--)
	{
		int exp = 1 << i;
		if(wall_width == 1 && wall_height >= exp)
		{
			int num_y = wall_height / exp;
			Static_Model* model = vert_models[i];
#ifdef DEBUG_SUBDIVIDE_WALL
			LOGI("Generated 1x%d 1x%d models. bounds:(%d,%d)",num_y,exp,wall_width,wall_height);
#endif
			//Generate 1 by num_y
			for(int k = 0; k < num_y; k++)
			{
				if(*mdl_count >= BUILDING_MAX_EXTERIOR_MODELS)
				{
					LOGW("Warning, exceeded max exterior model count of %d",*mdl_count);
					return;
				}
				mdl_list[*mdl_count] = model;
				trans_list[(*mdl_count)++] = trans * Mat4::TRANSLATE(Vec3(0,0,k*exp*WINDOW_TILE_SIZE));
			}

			//Getting the end point that we filled to:
			Vec3 ofs = Vec3(1, 0, num_y * exp);
			//If we did not fill the wall vertically
			if(wall_height - (num_y * exp) > 0)
			{
#ifdef DEBUG_SUBDIVIDE_WALL
				LOGI("Calling subdivide at top on:  %dx%d area at (0,0,%d)",wall_width, wall_height-(int)ofs.z,(int)ofs.z);
#endif
				//Generate from the left all the way to the right
				subdivide_wall(trans * Mat4::TRANSLATE(Vec3(0,0,ofs.z * WINDOW_TILE_SIZE)),1, wall_height-(int)ofs.z,mdl_count,mdl_list,trans_list);
			}
			return;
		}
		if(wall_height == 1 && wall_width >= exp)
		{
			int num_x = wall_width / exp;
			Static_Model* model = hor_models[i];
#ifdef DEBUG_SUBDIVIDE_WALL
			LOGI("Generated %dx1 %dx1 models. bounds:(%d,%d)",num_x,exp,wall_width,wall_height);
#endif
			//Generate num_x by 1 wall
			for(int j = 0; j < num_x; j++)
			{
				if(*mdl_count >= BUILDING_MAX_EXTERIOR_MODELS)
				{
					LOGW("Warning, exceeded max exterior model count of %d",*mdl_count);
					return;
				}
				mdl_list[*mdl_count] = model;
				trans_list[(*mdl_count)++] = trans * Mat4::TRANSLATE(Vec3(j*exp*TILE_SIZE,0,0));
			}

			//Getting the end point that we filled to:
			Vec3 ofs = Vec3(num_x * exp, 0, 1);
			//If we did not fill the wall horizontally
			if(wall_width - (num_x * exp) > 0)
			{
#ifdef DEBUG_SUBDIVIDE_WALL
				LOGI("Calling subdivide on right on:  %dx%d area at (%d,0,0)",wall_width-(int)ofs.x, (int)ofs.z,(int)ofs.x);
#endif
				//Generate from bottom to the top of the tiles that we placed
				subdivide_wall(trans * Mat4::TRANSLATE(Vec3(ofs.x * TILE_SIZE,0,0)),wall_width-(int)ofs.x, 1,mdl_count,mdl_list,trans_list);
			}
			return;
		}
		if(wall_width >= exp && wall_height >= exp && (wall_width > 1 && wall_height > 1))
		{
			int num_x = wall_width / exp;
			int num_y = wall_height / exp;

			Static_Model* model = models[i];
#ifdef DEBUG_SUBDIVIDE_WALL
			LOGI("Generated %dx%d %dx%d models. bounds:(%d,%d)",num_x,num_y,exp,exp,wall_width,wall_height);
#endif
			//Generate num_x by num_y wall
			for(int j = 0; j < num_x; j++)
			{
				for(int k = 0; k < num_y; k++)
				{
					if(*mdl_count >= BUILDING_MAX_EXTERIOR_MODELS)
					{
						LOGW("Warning, exceeded max exterior model count of %d",*mdl_count);
						return;
					}
					mdl_list[*mdl_count] = model;
					trans_list[(*mdl_count)++] = trans * Mat4::TRANSLATE(Vec3(j*exp*TILE_SIZE,0,k*exp*WINDOW_TILE_SIZE));
				}
			}

			//Getting the end point that we filled to:
			Vec3 ofs = Vec3(num_x * exp, 0, num_y * exp);


			//If we did not fill the wall vertically
			if(wall_height - (num_y * exp) > 0)
			{
#ifdef DEBUG_SUBDIVIDE_WALL
				LOGI("Calling subdivide at top on:  %dx%d area at (0,0,%d)",wall_width, wall_height-(int)ofs.z,(int)ofs.z);
#endif
				//Generate from the left all the way to the right
				subdivide_wall(trans * Mat4::TRANSLATE(Vec3(0,0,ofs.z * WINDOW_TILE_SIZE)),wall_width, wall_height-(int)ofs.z,mdl_count,mdl_list,trans_list);
			}
			//If we did not fill the wall horizontally
			if(wall_width - (num_x * exp) > 0)
			{
#ifdef DEBUG_SUBDIVIDE_WALL
				LOGI("Calling subdivide on right on:  %dx%d area at (%d,0,0)",wall_width-(int)ofs.x, (int)ofs.z,(int)ofs.x);
#endif
				//Generate from bottom to the top of the tiles that we placed
				subdivide_wall(trans * Mat4::TRANSLATE(Vec3(ofs.x * TILE_SIZE,0,0)),wall_width-(int)ofs.x, (int)ofs.z,mdl_count,mdl_list,trans_list);
			}
			return;
		}
	}
}

//Subdivide an interior window wall horizontally
void Building::subdivide_interior_wall(Mat4 trans, int wall_width, int *mdl_count, Static_Model **mdl_list, Mat4 *trans_list)
{
	Static_Model* hor_models[6];
	//TODO: change these references depending on the style
	hor_models[0] = Global_Tiles::instance->window_styles[0]->int_window_models->tile_model;
	hor_models[1] = Global_Tiles::instance->window_styles[0]->int_window_models->m1x2_model;
	hor_models[2] = Global_Tiles::instance->window_styles[0]->int_window_models->m1x4_model;
	hor_models[3] = Global_Tiles::instance->window_styles[0]->int_window_models->m1x8_model;
	hor_models[4] = Global_Tiles::instance->window_styles[0]->int_window_models->m1x16_model;
	hor_models[5] = Global_Tiles::instance->window_styles[0]->int_window_models->m1x32_model;

	for(int i = BUILDING_WINDOW_MAX_TILE_MATRIX; i >= 0; i--)
	{
		int exp = 1 << i;
		if(wall_width >= exp)
		{
			int num_x = wall_width / exp;
			Static_Model* model = hor_models[i];
			//Generate a num_x wide interior wall
			for(int j = 0; j < num_x; j++)
			{
				if(*mdl_count >= BUILDING_MAX_EXTERIOR_MODELS)
				{
					LOGW("Warning, exceeded max exterior model count of %d",*mdl_count);
					return;
				}
				mdl_list[*mdl_count] = model;
				trans_list[(*mdl_count)++] = trans * Mat4::TRANSLATE(Vec3(j*exp*TILE_SIZE,0,0));
			}

			//Getting the end point that we filled to:
			Vec3 ofs = Vec3(num_x * exp, 0, 1);
			//If we did not fill the wall horizontally
			if(wall_width - (num_x * exp) > 0)
			{
				//Generate from bottom to the top of the tiles that we placed
				subdivide_interior_wall(trans * Mat4::TRANSLATE(Vec3(ofs.x * TILE_SIZE,0,0)),wall_width-(int)ofs.x,mdl_count,mdl_list,trans_list);
			}
			return;
		}
	}
}

void Building::generate_exterior_model_list()
{
	//TODO: set exterior style model
	//Generating the front wall of the building
	Mat4 world_trans = Mat4::TRANSLATE(Vec3::ZERO());
	subdivide_wall(world_trans,(int)dimensions.x,(int)dimensions.z,&ext_mdl_fw_count,ext_mdls_fw,ext_mdl_fw_trans);

	//Generating the back wall of the building
	Mat4 wall_orient = world_trans * Mat4::TRANSLATE(Vec3(size.x,size.y,0)) * Mat4::ROTATE(Quat(PI,Vec3::UP()));
	subdivide_wall(wall_orient,(int)dimensions.x,(int)dimensions.z,&ext_mdl_bw_count,ext_mdls_bw,ext_mdl_bw_trans);

	//Generating the right wall of the building
	wall_orient = world_trans * Mat4::TRANSLATE(Vec3(size.x,0,0)) * Mat4::ROTATE(Quat(HALF_PI,Vec3::UP()));
	subdivide_wall(wall_orient,(int)dimensions.y,(int)dimensions.z,&ext_mdl_rw_count,ext_mdls_rw,ext_mdl_rw_trans);

	//Generating the left wall of the building
	wall_orient = world_trans * Mat4::TRANSLATE(Vec3(0,size.y,0)) * Mat4::ROTATE(Quat(HALF_PI+PI,Vec3::UP()));
	subdivide_wall(wall_orient,(int)dimensions.y,(int)dimensions.z,&ext_mdl_lw_count,ext_mdls_lw,ext_mdl_lw_trans);

	//LOGI("Exterior model list generation finished using %d (f%d,b%d,r%d,l%d) models",
	//ext_mdl_fw_count + ext_mdl_bw_count + ext_mdl_rw_count + ext_mdl_lw_count,
	//	ext_mdl_fw_count, ext_mdl_bw_count, ext_mdl_rw_count, ext_mdl_lw_count);
}


void Building::generate_interior_model_list()
{
	//TODO: set exterior style model
	//Generating the front inside wall of the building
	Mat4 world_trans = Mat4::TRANSLATE(Vec3(0,0,active_floor_number*WINDOW_TILE_SIZE));
	subdivide_interior_wall(world_trans,(int)dimensions.x,&int_mdl_fw_count,int_mdls_fw,int_mdl_fw_trans);

	//Generating the back inside wall of the building
	Mat4 wall_orient = world_trans * Mat4::TRANSLATE(Vec3(size.x,size.y,0)) * Mat4::ROTATE(Quat(PI,Vec3::UP()));
	subdivide_interior_wall(wall_orient,(int)dimensions.x,&int_mdl_bw_count,int_mdls_bw,int_mdl_bw_trans);

	//Generating the right inside wall of the building
	wall_orient = world_trans * Mat4::TRANSLATE(Vec3(size.x,0,0)) * Mat4::ROTATE(Quat(HALF_PI,Vec3::UP()));
	subdivide_interior_wall(wall_orient,(int)dimensions.y,&int_mdl_rw_count,int_mdls_rw,int_mdl_rw_trans);

	//Generating the left inside wall of the building
	wall_orient = world_trans * Mat4::TRANSLATE(Vec3(0,size.y,0)) * Mat4::ROTATE(Quat(HALF_PI+PI,Vec3::UP()));
	subdivide_interior_wall(wall_orient,(int)dimensions.y,&int_mdl_lw_count,int_mdls_lw,int_mdl_lw_trans);

	//LOGI("Interior model list generation finished using %d (f%d,b%d,r%d,l%d) models",
	//	int_mdl_fw_count + int_mdl_bw_count + int_mdl_rw_count + int_mdl_lw_count,
	//	int_mdl_fw_count, int_mdl_bw_count, int_mdl_rw_count, int_mdl_lw_count);
}


int Building::render_ext_walls(Mat4 vp)
{
	Mat4 bldg_ofs = Mat4::TRANSLATE(global_mins);
	Mat4 m;
	Mat4 mvp;
	Mat3 m_it;
	//Slight optimization: the way in which I populate this array gives a high probability
	// that two consecutive models are the same model.
	// Thus if the last model we rendered is the same as the current model, we can skip binding this model
	Static_Model* last_model = NULL;
	Static_Model* model;
	Material* mat = Global_Tiles::instance->window_styles[0]->variants[0]->ext_mat;

	//Rendering the all walls:
	int *mdl_count;
	Static_Model **mdl_list;
	Mat4 *trans_list;

	//Iteration arrays
	int *mdl_counts[4] = {&ext_mdl_fw_count,&ext_mdl_bw_count,&ext_mdl_lw_count,&ext_mdl_rw_count};
	Static_Model **mdl_lists[4] = {ext_mdls_fw,ext_mdls_bw,ext_mdls_lw,ext_mdls_rw};
	Mat4 *trans_lists[4] = {ext_mdl_fw_trans,ext_mdl_bw_trans,ext_mdl_lw_trans,ext_mdl_rw_trans};

	for(int i = 0; i < 4; i++)
	{
		mdl_count = mdl_counts[i];
		mdl_list = mdl_lists[i];
		trans_list = trans_lists[i];

		for(int j = 0; j < *mdl_count; j++)
		{
			model = mdl_list[j];
			if(model != last_model)
				model->bind_mesh_data2(mat);
			m = bldg_ofs * trans_list[j];
			mat->bind_value(Shader::PARAM_M_MATRIX, (void*) m.m);

			mvp = vp * m;
			mat->bind_value(Shader::PARAM_MVP_MATRIX, (void*) mvp.m);

			m_it = m.inverted_then_transposed().get_mat3();
			mat->bind_value(Shader::PARAM_M_IT_MATRIX, (void*) m_it.m);

			model->render_without_bind();
			last_model = model;
		}
	}

	return 1;
}

int Building::render_int_walls(Mat4 vp)
{
	Mat4 bldg_ofs = Mat4::TRANSLATE(global_mins);
	Mat4 m;
	Mat4 mvp;
	Mat3 m_it;
	//Slight optimization: the way in which I populate this array gives a high probability
	// that two consecutive models are the same model.
	// Thus if the last model we rendered is the same as the current model, we can skip binding this model
	Static_Model* last_model = NULL;
	Static_Model* model;
	Material* mat = Global_Tiles::instance->window_styles[0]->variants[0]->int_mat;

	//Rendering the all walls:
	int *mdl_count;
	Static_Model **mdl_list;
	Mat4 *trans_list;

	//Iteration arrays
	int *mdl_counts[4] = {&int_mdl_fw_count,&int_mdl_bw_count,&int_mdl_lw_count,&int_mdl_rw_count};
	Static_Model **mdl_lists[4] = {int_mdls_fw,int_mdls_bw,int_mdls_lw,int_mdls_rw};
	Mat4 *trans_lists[4] = {int_mdl_fw_trans,int_mdl_bw_trans,int_mdl_lw_trans,int_mdl_rw_trans};

	for(int i = 0; i < 4; i++)
	{
		mdl_count = mdl_counts[i];
		mdl_list = mdl_lists[i];
		trans_list = trans_lists[i];

		for(int j = 0; j < *mdl_count; j++)
		{
			model = mdl_list[j];
			if(model != last_model)
				model->bind_mesh_data2(mat);
			m = bldg_ofs * trans_list[j];
			mat->bind_value(Shader::PARAM_M_MATRIX, (void*) m.m);

			mvp = vp * m;
			mat->bind_value(Shader::PARAM_MVP_MATRIX, (void*) mvp.m);

			m_it = m.inverted_then_transposed().get_mat3();
			mat->bind_value(Shader::PARAM_M_IT_MATRIX, (void*) m_it.m);

			model->render_without_bind();
			last_model = model;
		}
	}
	return 1;
}

int Building::render(Vec3 player_pos, Mat4 vp)
{
	if(!generated)
	{
		return 1;
	}
	//If we are in the building, only render floor and interior glass
	//if we are out of the building, only render exterior
	bool plyr_in_bldg = !is_out_of_bounds(player_pos);

	if(!plyr_in_bldg)
	{
		Global_Tiles::instance->window_styles[0]->variants[0]->bind_variant_ext();
		Material* mat = Global_Tiles::instance->window_styles[0]->variants[0]->ext_mat;

		//Using Skybox cubemap
		mat->bind_value(Shader::PARAM_CUBE_MAP,(void*) Global_Tiles::instance->sky_cube_map);

		//Should I render walls independently and cull non-visible ones?
		render_ext_walls(vp);
	}

	bool render_floor = (active_floor && is_in_bounds_or_near_front(player_pos));
	//Don't render the floor unless we are in the building, or the player is within 5 meters of the front of the building

	if(render_floor)
	{
		active_floor->render(vp);
	}
	return 1;
}

//Rendering method called at the end to render transparent windows and tiles
int Building::render_transparent_meshes(Vec3 player_pos, Mat4 vp)
{
	if(!generated)
	{
		return 1;
	}

	if(broken_owindow_active)
	{
		Global_Tiles::instance->window_styles[0]->variants[0]->bind_variant_int_skel();
		Material* mat = Global_Tiles::instance->window_styles[0]->variants[0]->int_skel_mat;

		//Using the interior reflective cubemap
		mat->bind_value(Shader::PARAM_CUBE_MAP,(void*) Global_Tiles::instance->tile_styles[0]->variants[0]->ref_cube_map);

		Skel_Model* model = Global_Tiles::instance->window_styles[0]->broken_out_window;
		model->render_sans_weights(Mat4::IDENTITY(),vp,mat,broken_owindow_skel);
	}
	if(broken_iwindow_active)
	{
		Global_Tiles::instance->window_styles[0]->variants[0]->bind_variant_ext_skel();
		Material* mat = Global_Tiles::instance->window_styles[0]->variants[0]->ext_skel_mat;

		//Using Skybox cubemap
		mat->bind_value(Shader::PARAM_CUBE_MAP,(void*) Global_Tiles::instance->sky_cube_map);

		Skel_Model* model = Global_Tiles::instance->window_styles[0]->broken_in_window;
		model->render_sans_weights(Mat4::IDENTITY(),vp,mat,broken_iwindow_skel);
	}

	bool render_inner_window = (is_in_bounds_or_near_front(player_pos));

	//Only render the interior windows if we are in the building
	if(render_inner_window)
	{

		Global_Tiles::instance->window_styles[0]->variants[0]->bind_variant_int();
		Material* mat = Global_Tiles::instance->window_styles[0]->variants[0]->int_mat;

		//Using Floor interior cubemap
		//FIXME: make sure we reference the correct style and variant for the floor style and variant
		mat->bind_value(Shader::PARAM_CUBE_MAP,(void*) Global_Tiles::instance->tile_styles[0]->variants[0]->ref_cube_map);

		render_int_walls(vp);
	}


	//if we end up using transparency in tiles, call transparent render for floor
	//if(active_floor)
	//	active_floor->render(vp);
	return 1;
}

//Sets the window that player_pos hits as broken. (direction: true = into the building, false = out of the building)
void Building::break_window(Vec3 player_pos, bool into_building)
{
	//Finding pos relative to building global min
	player_pos = player_pos - global_mins;

	int window_x = (int) (player_pos.x / TILE_SIZE);
	int window_y = (int) (player_pos.z / WINDOW_TILE_SIZE);

	//TODO: make sure we use the correct exterior tile model tiles

	if(into_building)
	{
		broken_iwindow_index_x = window_x;
		broken_iwindow_index_y = window_y;

		//Resetting the building exterior and interior wall shell:
		ext_mdl_fw_count = 0;
		int_mdl_fw_count = 0;

		Mat4 window_lev = Mat4::TRANSLATE(Vec3(0,0,window_y*WINDOW_TILE_SIZE));
		Mat4 trans_ext = Mat4::TRANSLATE(Vec3::ZERO());
		Mat4 trans_ext_left = Mat4::TRANSLATE(Vec3(0,0,0)) * window_lev;
		Mat4 trans_ext_above = Mat4::TRANSLATE(Vec3(0,0,WINDOW_TILE_SIZE)) * window_lev;
		Mat4 trans_ext_right = Mat4::TRANSLATE(Vec3((window_x+1)*TILE_SIZE,0,0)) * window_lev;

		Mat4 trans_int_left = window_lev;
		Mat4 trans_int_right = Mat4::TRANSLATE(Vec3((window_x+1)*TILE_SIZE,0,0)) * window_lev;

		//Populating wall mesh below the broken window
		subdivide_wall(trans_ext,(int)dimensions.x,window_y,&ext_mdl_fw_count,ext_mdls_fw,ext_mdl_fw_trans);
		//Populating wall mesh to left of broken window
		subdivide_wall(trans_ext_left,window_x,1,&ext_mdl_fw_count,ext_mdls_fw,ext_mdl_fw_trans);
		//Populating wall mesh to right of broken window
		subdivide_wall(trans_ext_right,(int)dimensions.x-(window_x+1),1,&ext_mdl_fw_count,ext_mdls_fw,ext_mdl_fw_trans);
		//Populating wall mesh above the broken window
		subdivide_wall(trans_ext_above,(int)dimensions.x,(int)dimensions.z-(window_y+1),
			&ext_mdl_fw_count,ext_mdls_fw,ext_mdl_fw_trans);
		//Populating inside wall to left of broken window
		subdivide_interior_wall(trans_int_left,window_x,&int_mdl_fw_count,int_mdls_fw,int_mdl_fw_trans);
		//Populating inside wall to right of broken window
		subdivide_interior_wall(trans_int_right,(int)dimensions.x-(window_x+1),
			&int_mdl_fw_count,int_mdls_fw,int_mdl_fw_trans);


		//Setting up the broken window tile
		broken_iwindow_active = true;
		broken_iwindow_skel->pos = Vec3(window_x * TILE_SIZE, 0, window_y * WINDOW_TILE_SIZE);
		broken_iwindow_skel->angles = Vec3::ZERO();
		broken_iwindow_skel->play_anim(WINDOW_ANIM_BREAK,ANIM_END_TYPE_LOOP);
	}
	//Breaking window out of building
	else
	{
		broken_owindow_index_x = window_x;
		broken_owindow_index_y = window_y;

		//Because window_x is the index from the our left of the building
		//we want the index from the right of the back wall (which is index 0):
		window_x = (int)(dimensions.x - 1 - window_x);

		//Resetting the building exterior and interior wall shell:
		ext_mdl_bw_count = 0;
		int_mdl_bw_count = 0;

		Mat4 window_lev = Mat4::TRANSLATE(Vec3(size.x,size.y,window_y*WINDOW_TILE_SIZE))
					  * Mat4::ROTATE(Quat(PI,Vec3::UP()));

		Mat4 trans_ext = Mat4::TRANSLATE(Vec3(size.x,size.y,0)) * Mat4::ROTATE(Quat(PI,Vec3::UP()));
		Mat4 trans_ext_left = window_lev * Mat4::TRANSLATE(Vec3(0,0,0));
		Mat4 trans_ext_above = window_lev * Mat4::TRANSLATE(Vec3(0,0,WINDOW_TILE_SIZE));
		Mat4 trans_ext_right = window_lev * Mat4::TRANSLATE(Vec3((window_x+1)*TILE_SIZE,0,0));

		Mat4 trans_int_left = window_lev * Mat4::TRANSLATE(Vec3(0,0,0));
		Mat4 trans_int_right = window_lev * Mat4::TRANSLATE(Vec3((window_x+1)*TILE_SIZE,0,0));

		//Populating wall mesh below the broken window
		subdivide_wall(trans_ext,(int)dimensions.x,window_y,&ext_mdl_bw_count,ext_mdls_bw,ext_mdl_bw_trans);
		//Populating wall mesh to left of broken window
		subdivide_wall(trans_ext_left,window_x,1,&ext_mdl_bw_count,ext_mdls_bw,ext_mdl_bw_trans);
		//Populating wall mesh to right of broken window
		subdivide_wall(trans_ext_right,(int)dimensions.x-(window_x+1),1,&ext_mdl_bw_count,ext_mdls_bw,ext_mdl_bw_trans);
		//Populating wall mesh above the broken window
		subdivide_wall(trans_ext_above,(int)dimensions.x,(int)dimensions.z-(window_y+1),
			&ext_mdl_bw_count,ext_mdls_bw,ext_mdl_bw_trans);
		//Populating inside wall to left of broken window
		subdivide_interior_wall(trans_int_left,window_x,&int_mdl_bw_count,int_mdls_bw,int_mdl_bw_trans);
		//Populating inside wall to right of broken window
		subdivide_interior_wall(trans_int_right,(int)dimensions.x-(window_x+1),
			&int_mdl_bw_count,int_mdls_bw,int_mdl_bw_trans);

		//Setting up the broken window tile
		broken_owindow_active = true;
		broken_owindow_skel->pos = Vec3(((broken_owindow_index_x+1)*TILE_SIZE),global_maxs.y-global_mins.y,window_y*WINDOW_TILE_SIZE);
		broken_owindow_skel->angles = Vec3(0,PI,0);
		broken_owindow_skel->play_anim(WINDOW_ANIM_BREAK,ANIM_END_TYPE_LOOP);
	}
}

//Returns the floor number p is in
int Building::get_floor_num_at_pos(Vec3 p)
{
	p = p - global_mins;
	return (int) (p.z / WINDOW_TILE_SIZE);
}

//Offsets the building and all of the building's data
void Building::offset_building(Vec3 ofs)
{
	pos += ofs;
	global_mins += ofs;
	global_maxs += ofs;

	active_floor->global_pos += ofs;
	active_floor->global_mins += ofs;
	active_floor->global_maxs += ofs;
}

//Updated any game logic that the building may need to handle
void Building::update()
{
	if(broken_iwindow_active)
	{
		broken_iwindow_skel->update_frame();
	}
	if(broken_owindow_active)
	{
		broken_owindow_skel->update_frame();
	}
}
