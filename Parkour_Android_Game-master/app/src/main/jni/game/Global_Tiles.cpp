//
// Created by F1 on 9/3/2016.
//

#include "Global_Tiles.hpp"


Interior_Style::Interior_Style()
{
	variants[0] = new Interior_Variant();

	tiles[0] = new Grid_Tile*[2];
	type_variant_counts[0] = 2;
	tiles[1] = new Grid_Tile*[3];
	type_variant_counts[1] = 3;

	//Initializing obstacle subtypes to null pointers
	for(int i = 0; i < TILE_OBSTACLE_COUNT; i++)
	{
		obst_tiles[i] = NULL;
	}

	//Initializing wall_subtypes to null pointers
	for(int i = 0; i < WALL_TYPE_COUNT; i++)
	{
		wall_subtypes[i] = NULL;
	}
	//Initializing rail_subtypes to null pointers
	for(int i = 0; i < RAIL_TYPE_COUNT; i++)
	{
		rail_subtypes[i] = NULL;
	}
}

Interior_Style::~Interior_Style()
{
	delete variants[0];
	for(int i = 0; i < TILE_TYPES; i++)
	{
		delete[] tiles[i];
	}
}


Exterior_Style::Exterior_Style()
{
	variants[0] = new Exterior_Variant();
}

Exterior_Style::~Exterior_Style()
{
	delete variants[0];
}

int Global_Tiles::init_data()
{
	GLuint shader_ptypes[] =
	{
	Shader::PARAM_VERTICES,
	Shader::PARAM_VERT_UV1,
	Shader::PARAM_VERT_UV2,
	Shader::PARAM_VERT_NORMALS,
	Shader::PARAM_VERT_BINORMALS,
	Shader::PARAM_VERT_TANGENTS,
	Shader::PARAM_MVP_MATRIX,
	Shader::PARAM_M_MATRIX,
	Shader::PARAM_M_IT_MATRIX,
	Shader::PARAM_TEXTURE_DIFFUSE,
	Shader::PARAM_TEXTURE_NORMAL,
	Shader::PARAM_TEXTURE_MISC,
	Shader::PARAM_TEXTURE_LIGHTMAP,
	Shader::PARAM_CUBE_MAP
	};
	const char *shader_pnames[] =
	{
	"vert_pos",
	"vert_uv_1",
	"vert_uv_2",
	"vert_nor",
	"vert_binor",
	"vert_tan",
	"mvp",
	"m",
	"m_IT",
	"tex_diff",
	"tex_nor",
	"tex_misc",
	"tex_lm",
	"cube_map"
	};

	Interior_Variant::init_static_data("shaders/bldg_int.vert","shaders/bldg_int.frag",shader_ptypes,shader_pnames,14);

	//Initializing Exterior Variant static shader data
	//Exterior Window shader parameters
	GLuint ptypes2[] =
	{
	Shader::PARAM_VERTICES,
	Shader::PARAM_VERT_UV1,
	Shader::PARAM_VERT_NORMALS,
	Shader::PARAM_VERT_BINORMALS,
	Shader::PARAM_VERT_TANGENTS,
	Shader::PARAM_MVP_MATRIX,
	Shader::PARAM_M_MATRIX,
	Shader::PARAM_M_IT_MATRIX,
	Shader::PARAM_TEXTURE_DIFFUSE,
	Shader::PARAM_TEXTURE_NORMAL,
	Shader::PARAM_TEXTURE_MISC,
	Shader::PARAM_CUBE_MAP
	};
	const char *pnames2[] =
	{
	"vert_pos",
	"vert_uv_1",
	"vert_nor",
	"vert_binor",
	"vert_tan",
	"mvp",
	"m",
	"m_IT",
	"tex_diff",
	"tex_nor",
	"tex_misc",
	"cube_map"
	};

	//Interior and exterior shaders have the same parameters
	Exterior_Variant::init_static_ext_shader("shaders/bldgwin.vert","shaders/bldgwin_ext.frag",ptypes2,pnames2,12);
	Exterior_Variant::init_static_int_shader("shaders/bldgwin.vert","shaders/bldgwin_int.frag",ptypes2,pnames2,12);

	//Skeletal versions of the above shaders:
	GLuint ptypes3[] =
	{
	Shader::PARAM_VERTICES,
	Shader::PARAM_VERT_UV1,
	Shader::PARAM_VERT_NORMALS,
	Shader::PARAM_VERT_BINORMALS,
	Shader::PARAM_VERT_TANGENTS,
	Shader::PARAM_MVP_MATRIX,
	Shader::PARAM_M_MATRIX,
	Shader::PARAM_M_IT_MATRIX,
	Shader::PARAM_TEXTURE_DIFFUSE,
	Shader::PARAM_TEXTURE_NORMAL,
	Shader::PARAM_TEXTURE_MISC,
	Shader::PARAM_CUBE_MAP,
	Shader::PARAM_BONE_INDICES,
	//Shader::PARAM_BONE_WEIGHTS,
	Shader::PARAM_BONE_MATRICES,
	Shader::PARAM_BONE_IT_MATRICES
	};
	const char *pnames3[] =
	{
	"vert_pos",
	"vert_uv_1",
	"vert_nor",
	"vert_binor",
	"vert_tan",
	"mvp",
	"m",
	"m_IT",
	"tex_diff",
	"tex_nor",
	"tex_misc",
	"cube_map",
	"bone_index",
	//"bone_weight",
	"bone",
	"bone_IT"
	};

	Exterior_Variant::init_static_ext_skel_shader("shaders/bldgwin_skel.vert","shaders/bldgwin_ext.frag",ptypes3,pnames3,15);
	Exterior_Variant::init_static_int_skel_shader("shaders/bldgwin_skel.vert","shaders/bldgwin_int.frag",ptypes3,pnames3,15);

	instance = new Global_Tiles();

	return 1;
}

void Global_Tiles::term_data()
{
	delete instance;

	Interior_Variant::term_static_data();
	Exterior_Variant::term_static_data();
}

Global_Tiles::Global_Tiles()
{
	//sky_cube_map = new Cube_Map("cube_maps/sky_cm.pkm",512);
	sky_cube_map = new Cube_Map("cube_maps/sky.data",512);

	tile_styles[0] = new Interior_Style();

	tile_styles[0]->variants[0]->diffuse_map = new Texture("textures/tiles/s0v1_diff.pkm",2048,2048);
	tile_styles[0]->variants[0]->normal_map = new Texture("textures/tiles/s0v1_nor.pkm",2048,2048);
	tile_styles[0]->variants[0]->misc_map = new Texture("textures/tiles/s0v1_misc.pkm",2048,2048);
	tile_styles[0]->variants[0]->light_map = new Texture("textures/tiles/s0_lm.pkm",2048,2048);
	//tile_styles[0]->variants[0]->ref_cube_map = new Cube_Map("cube_maps/sky_cm.pkm",512);
	tile_styles[0]->variants[0]->ref_cube_map = new Cube_Map("cube_maps/sky.data",512);

	//tile_styles[0]->type[0]->model->load_model();
	//tile_styles[0]->type[0]->collision_map = new Collision_Map();

	//Empty tile
	tile_styles[0]->empty_tile = new Grid_Tile(0,0);
	tile_styles[0]->empty_tile->model = new Static_Model("models/tiles/style0/empt0.stmf");

	tile_styles[0]->obst_tiles[0] = new Grid_Tile(1,0);
	tile_styles[0]->obst_tiles[1] = new Grid_Tile(1,0);
	tile_styles[0]->obst_tiles[2] = new Grid_Tile(0,0);
	tile_styles[0]->obst_tiles[3] = new Grid_Tile(0,0);
	tile_styles[0]->obst_tiles[4] = new Grid_Tile(1,0);
	tile_styles[0]->obst_tiles[5] = new Grid_Tile(1,0);
	tile_styles[0]->obst_tiles[6] = new Grid_Tile(1,0);
	tile_styles[0]->obst_tiles[7] = new Grid_Tile(1,0);
	tile_styles[0]->obst_tiles[8] = new Grid_Tile(1,0);
	tile_styles[0]->obst_tiles[9] = new Grid_Tile(1,0);

	//Loading obstacle tile models:
	tile_styles[0]->obst_tiles[0]->model = new Static_Model("models/tiles/style0/obst_0.stmf");
	tile_styles[0]->obst_tiles[1]->model = new Static_Model("models/tiles/style0/obst_1.stmf");
	tile_styles[0]->obst_tiles[2]->model = new Static_Model("models/tiles/style0/obst_2.stmf");
	tile_styles[0]->obst_tiles[3]->model = new Static_Model("models/tiles/style0/obst_3.stmf");
	tile_styles[0]->obst_tiles[4]->model = new Static_Model("models/tiles/style0/obst_4.stmf");
	tile_styles[0]->obst_tiles[5]->model = new Static_Model("models/tiles/style0/obst_5.stmf");
	tile_styles[0]->obst_tiles[6]->model = new Static_Model("models/tiles/style0/obst_6.stmf");
	tile_styles[0]->obst_tiles[7]->model = new Static_Model("models/tiles/style0/obst_7.stmf");
	tile_styles[0]->obst_tiles[8]->model = new Static_Model("models/tiles/style0/obst_8.stmf");
	tile_styles[0]->obst_tiles[9]->model = new Static_Model("models/tiles/style0/obst_9.stmf");


	//=============== Obstacle Collision Maps =====================
	//Setting middle row as solid for all collision maps that have the center wall
	for(int i = 0; i < TILE_VOXEL_DIMS; i++)
	{
		tile_styles[0]->obst_tiles[0]->coll_map->voxel[i][3] = CLIP_SOLID;
		tile_styles[0]->obst_tiles[1]->coll_map->voxel[i][3] = CLIP_SOLID;
		tile_styles[0]->obst_tiles[5]->coll_map->voxel[i][3] = CLIP_SOLID;
		tile_styles[0]->obst_tiles[8]->coll_map->voxel[i][3] = CLIP_SOLID;
		tile_styles[0]->obst_tiles[9]->coll_map->voxel[i][3] = CLIP_SOLID;
	}

	//Slide through air duct tile [0]
		//Center wall
	tile_styles[0]->obst_tiles[0]->coll_map->voxel[3][4] = CLIP_SLIDE;
	tile_styles[0]->obst_tiles[0]->coll_map->voxel[3][3] = CLIP_SLIDE;

	//Dive through air duct tile [1]
		//Center wall
	//Run through doorways [2]
	tile_styles[0]->obst_tiles[2]->coll_map->voxel[0][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[2]->coll_map->voxel[1][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[2]->coll_map->voxel[5][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[2]->coll_map->voxel[6][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[2]->coll_map->voxel[1][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[2]->coll_map->voxel_shape[1][2] = CLIP_SHAPE_GT_POS;
	tile_styles[0]->obst_tiles[2]->coll_map->voxel[5][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[2]->coll_map->voxel_shape[5][2] = CLIP_SHAPE_GT_NEG;
	tile_styles[0]->obst_tiles[2]->coll_map->voxel[2][3] = CLIP_DOORWAY;
	tile_styles[0]->obst_tiles[2]->coll_map->voxel[3][3] = CLIP_DOORWAY;
	tile_styles[0]->obst_tiles[2]->coll_map->voxel[4][3] = CLIP_DOORWAY;
	//Run through elevator [3]
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[0][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[6][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[1][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[5][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[1][5] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[5][5] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[1][4] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel_shape[1][4] = CLIP_SHAPE_GT_POS;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[5][4] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel_shape[5][4] = CLIP_SHAPE_GT_NEG;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[2][2] = CLIP_DOORWAY;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[3][2] = CLIP_DOORWAY;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[4][2] = CLIP_DOORWAY;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[2][5] = CLIP_DOORWAY;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[3][5] = CLIP_DOORWAY;
	tile_styles[0]->obst_tiles[3]->coll_map->voxel[4][5] = CLIP_DOORWAY;
	//Vault through wall [4]
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[0][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[6][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[1][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[2][2] = CLIP_MID;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[3][2] = CLIP_MID;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[4][2] = CLIP_MID;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[5][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[1][5] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel_shape[1][5] = CLIP_SHAPE_GT_POS;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[5][5] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel_shape[5][5] = CLIP_SHAPE_GT_NEG;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[2][5] = CLIP_DOORWAY;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[3][5] = CLIP_DOORWAY;
	tile_styles[0]->obst_tiles[4]->coll_map->voxel[4][5] = CLIP_DOORWAY;
	//slide over TV [5]
		//Center wall
	tile_styles[0]->obst_tiles[5]->coll_map->voxel[2][2] = CLIP_MID;
	tile_styles[0]->obst_tiles[5]->coll_map->voxel[3][2] = CLIP_MID;
	tile_styles[0]->obst_tiles[5]->coll_map->voxel[4][2] = CLIP_MID;
	tile_styles[0]->obst_tiles[5]->coll_map->voxel[2][4] = CLIP_MID;
	tile_styles[0]->obst_tiles[5]->coll_map->voxel[3][4] = CLIP_MID;
	tile_styles[0]->obst_tiles[5]->coll_map->voxel[4][4] = CLIP_MID;
	//Kong over desks [6]
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[0][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[6][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[1][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[1][5] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[5][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[5][5] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[2][2] = CLIP_MID;
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[3][2] = CLIP_MID;
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[4][2] = CLIP_MID;
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[2][5] = CLIP_MID;
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[3][5] = CLIP_MID;
	tile_styles[0]->obst_tiles[6]->coll_map->voxel[4][5] = CLIP_MID;
	//jump into elevator vent [7]
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[0][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[6][3] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[1][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[5][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[1][5] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[5][5] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[1][4] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel_shape[1][4] = CLIP_SHAPE_GT_POS;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[5][4] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel_shape[5][4] = CLIP_SHAPE_GT_NEG;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[2][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[3][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[4][2] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[2][5] = CLIP_DOORWAY;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[3][5] = CLIP_DOORWAY;
	tile_styles[0]->obst_tiles[7]->coll_map->voxel[4][5] = CLIP_DOORWAY;
	//dash vault over decor [8]
		//Center wall
	tile_styles[0]->obst_tiles[8]->coll_map->voxel[1][4] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[8]->coll_map->voxel[2][4] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[8]->coll_map->voxel[3][4] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[8]->coll_map->voxel[4][4] = CLIP_SOLID;
	tile_styles[0]->obst_tiles[8]->coll_map->voxel[5][4] = CLIP_SOLID;
	//swing through bookshelf [9]
		//Center wall
	//=============================================================

	//==================== Loading wall tiles =====================

	for(int i = 1; i < WALL_TYPE_COUNT; i++)
	{
		tile_styles[0]->wall_subtypes[i] = new Grid_Tile(0,0);
	}
	tile_styles[0]->wall_subtypes[WALL_TYPE_oXoY]->model = new Static_Model("models/tiles/style0/wall_oXoY.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_xooY]->model = new Static_Model("models/tiles/style0/wall_xooY.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_xoyo]->model = new Static_Model("models/tiles/style0/wall_xoyo.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_oXyo]->model = new Static_Model("models/tiles/style0/wall_oXyo.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_xXoo]->model = new Static_Model("models/tiles/style0/wall_xXoo.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_ooyY]->model = new Static_Model("models/tiles/style0/wall_ooyY.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_xXyY]->model = new Static_Model("models/tiles/style0/wall_xXyY.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_xoyY]->model = new Static_Model("models/tiles/style0/wall_xoyY.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_oXyY]->model = new Static_Model("models/tiles/style0/wall_oXyY.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_xXyo]->model = new Static_Model("models/tiles/style0/wall_xXyo.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_xXoY]->model = new Static_Model("models/tiles/style0/wall_xXoY.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_xooo]->model = new Static_Model("models/tiles/style0/wall_xooo.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_oXoo]->model = new Static_Model("models/tiles/style0/wall_oXoo.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_ooyo]->model = new Static_Model("models/tiles/style0/wall_ooyo.stmf");
	tile_styles[0]->wall_subtypes[WALL_TYPE_oooY]->model = new Static_Model("models/tiles/style0/wall_oooY.stmf");

	//Setting collision maps for wall subtype tiles
	const int center = 3;
	for(int i = 0; i <= center; i++)
	{
		for(int j = 1; j < WALL_TYPE_COUNT; j++)
		{
			//Setting the -Y axis wall as solid for all wall types that have the -Y axis wall
			if(j & WALL_TYPE_ooyo)
				tile_styles[0]->wall_subtypes[j]->coll_map->voxel[center][i] = CLIP_SOLID;

			//Setting the +Y axis wall as solid for all wall types that have the +Y axis wall
			if(j & WALL_TYPE_oooY)
				tile_styles[0]->wall_subtypes[j]->coll_map->voxel[center][center + i] = CLIP_SOLID;

			//Setting the -X axis wall as solid for all wall types that have the -X axis wall
			if(j & WALL_TYPE_xooo)
				tile_styles[0]->wall_subtypes[j]->coll_map->voxel[i][center] = CLIP_SOLID;

			//Setting the +X axis wall as solid for all wall types that have the +X axis wall
			if(j & WALL_TYPE_oXoo)
				tile_styles[0]->wall_subtypes[j]->coll_map->voxel[center + i][center] = CLIP_SOLID;
		}
	}
	//Don't have to worry about center voxel, because the above wall segments overlap


	for(int i = 1; i < RAIL_TYPE_COUNT; i++)
	{
		if(RAIL_TYPE_IS_VALID[i])
			tile_styles[0]->rail_subtypes[i] = new Grid_Tile(0,0);
	}

	//Loading rail models
	tile_styles[0]->rail_subtypes[RAIL_TYPE_L ]->model = new Static_Model("models/tiles/style0/rail_l.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_R ]->model = new Static_Model("models/tiles/style0/rail_r.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TL ]->model = new Static_Model("models/tiles/style0/rail_tl.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2]->model = new Static_Model("models/tiles/style0/rail_tl2.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TR ]->model = new Static_Model("models/tiles/style0/rail_tr.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TR2 ]->model = new Static_Model("models/tiles/style0/rail_tr2.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_LR ]->model = new Static_Model("models/tiles/style0/rail_lr.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_TR ]->model = new Static_Model("models/tiles/style0/rail_tl_tr.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_TR2]->model = new Static_Model("models/tiles/style0/rail_tl2_tr2.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_TL2]->model = new Static_Model("models/tiles/style0/rail_tl_tl2.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TR_TR2]->model = new Static_Model("models/tiles/style0/rail_tr_tr2.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_L]->model = new Static_Model("models/tiles/style0/rail_tl_l.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_R]->model = new Static_Model("models/tiles/style0/rail_tl2_r.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TR_R ]->model = new Static_Model("models/tiles/style0/rail_tr_r.stmf");
	tile_styles[0]->rail_subtypes[RAIL_TYPE_TR2_L ]->model = new Static_Model("models/tiles/style0/rail_tr2_l.stmf");

	//Setting up collision maps for rail tiles
	for(int i = 0; i < TILE_VOXEL_DIMS; i++)
	{
		//Setting the left rail as solid
		tile_styles[0]->rail_subtypes[RAIL_TYPE_L]->coll_map->voxel[6][i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_LR]->coll_map->voxel[6][i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_L]->coll_map->voxel[6][i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR2_L]->coll_map->voxel[6][i] = CLIP_SOLID;

		//Setting the right rail as solid
		tile_styles[0]->rail_subtypes[RAIL_TYPE_R]->coll_map->voxel[0][i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_LR]->coll_map->voxel[0][i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_R]->coll_map->voxel[0][i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR_R]->coll_map->voxel[0][i] = CLIP_SOLID;

		//Setting the TR rail
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR]->coll_map->voxel[i][i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR]->coll_map->voxel_shape[i][i] = CLIP_SHAPE_GT_POS;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR_R]->coll_map->voxel[i][i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR_R]->coll_map->voxel_shape[i][i] = CLIP_SHAPE_GT_POS;
		if(i > 0)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TR]->coll_map->voxel[i-1][i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TR]->coll_map->voxel_shape[i-1][i] = CLIP_SHAPE_LT_POS;
		}
		if(i > 1)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TR_R]->coll_map->voxel[i-1][i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TR_R]->coll_map->voxel_shape[i-1][i] = CLIP_SHAPE_LT_POS;
		}

		//Setting TR2 rail
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR2]->coll_map->voxel[i][i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR2]->coll_map->voxel_shape[i][i] = CLIP_SHAPE_LT_POS;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR2_L]->coll_map->voxel[i][i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR2_L]->coll_map->voxel_shape[i][i] = CLIP_SHAPE_LT_POS;
		if(i < 6)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TR2]->coll_map->voxel[i+1][i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TR2]->coll_map->voxel_shape[i+1][i] = CLIP_SHAPE_GT_POS;
		}
		if(i < 5)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TR2_L]->coll_map->voxel[i+1][i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TR2_L]->coll_map->voxel_shape[i+1][i] = CLIP_SHAPE_GT_POS;
		}

		//Setting TL rail
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL]->coll_map->voxel[i][6-i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL]->coll_map->voxel_shape[i][6-i] = CLIP_SHAPE_GT_NEG;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_L]->coll_map->voxel[i][6-i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_L]->coll_map->voxel_shape[i][6-i] = CLIP_SHAPE_GT_NEG;
		if(i < 6)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL]->coll_map->voxel[i+1][6-i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL]->coll_map->voxel_shape[i+1][6-i] = CLIP_SHAPE_LT_NEG;
		}
		if(i < 5)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_L]->coll_map->voxel[i+1][6-i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_L]->coll_map->voxel_shape[i+1][6-i] = CLIP_SHAPE_LT_NEG;
		}

		//Setting TL2 rail
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2]->coll_map->voxel[i][6-i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2]->coll_map->voxel_shape[i][6-i] = CLIP_SHAPE_LT_NEG;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_R]->coll_map->voxel[i][6-i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_R]->coll_map->voxel_shape[i][6-i] = CLIP_SHAPE_LT_NEG;
		if(i > 0)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2]->coll_map->voxel[i-1][6-i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2]->coll_map->voxel_shape[i-1][6-i] = CLIP_SHAPE_GT_NEG;
		}
		if(i > 1)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_R]->coll_map->voxel[i-1][6-i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_R]->coll_map->voxel_shape[i-1][6-i] = CLIP_SHAPE_GT_NEG;
		}

		//Setting TR & TR2 tile
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR_TR2]->coll_map->voxel[i][i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TR_TR2]->coll_map->voxel_shape[i][i] = CLIP_SHAPE_IN_WALL_POS;
		//Setting TL & TL2 tile
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_TL2]->coll_map->voxel[i][6-i] = CLIP_SOLID;
		tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_TL2]->coll_map->voxel_shape[i][6-i] = CLIP_SHAPE_IN_WALL_NEG;



		//Setting the TR & TL tile
		if(i < 3)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_TR]->coll_map->voxel[i][6-i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_TR]->coll_map->voxel_shape[i][6-i] = CLIP_SHAPE_GT_NEG;
		}
		else if(i == 3)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_TR]->coll_map->voxel[i][i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_TR]->coll_map->voxel_shape[i][i] = CLIP_SHAPE_GT_ABS;
		}
		else
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_TR]->coll_map->voxel[i][i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL_TR]->coll_map->voxel_shape[i][i] = CLIP_SHAPE_GT_POS;
		}
		//Setting the TR2 and TL2 tile
		if(i < 3)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_TR2]->coll_map->voxel[i][i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_TR2]->coll_map->voxel_shape[i][i] = CLIP_SHAPE_LT_POS;
		}
		else if(i == 3)
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_TR2]->coll_map->voxel[i][i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_TR2]->coll_map->voxel_shape[i][i] = CLIP_SHAPE_LT_ABS;
		}
		else
		{
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_TR2]->coll_map->voxel[i][6-i] = CLIP_SOLID;
			tile_styles[0]->rail_subtypes[RAIL_TYPE_TL2_TR2]->coll_map->voxel_shape[i][6-i] = CLIP_SHAPE_LT_NEG;
		}
	}


	//=============================================================

	//======================================================================================================================
	//================================================ Obstacle Maneuvers ==================================================
	//======================================================================================================================

	Keyframe** frames;

	//=================================================
	//Slide through air duct tile [0]
	//=================================================
	tile_styles[0]->obst_tiles[0]->maneuvers[0] = new Maneuver(4);
	tile_styles[0]->obst_tiles[0]->maneuvers[0]->set_input(INPUT_SWIPE_DOWN);
	frames = tile_styles[0]->obst_tiles[0]->maneuvers[0]->keyframes;
	//Begin slide, move to center
	frames[0]->set_bounds(Vec3(1,-0.5f,0),Vec3(2.5,1,0));
	frames[0]->set_speed(PLAYER_RUN_SPEED-2.0f,FRAME_SPEED_CONST_TIME);
	frames[0]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,2.0,0),PLAYER_TURN_LERP_FACTOR);
	frames[0]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_SLIDE,ANIM_END_TYPE_FREEZE);
	frames[0]->set_vbob(CAM_VIEWBOB_RUNNING);
	//Continue slide through
	frames[1]->set_bounds(Vec3(1.75,1.1,0));
	frames[1]->set_vbob(CAM_VIEWBOB_SLIDING);
	frames[1]->set_speed(PLAYER_RUN_SPEED-1.0f,PLAYER_SLIDE_ACCEL,PLAYER_SLIDE_MIN_SPEED);
	//Get up from slide
	frames[2]->set_bounds(Vec3(1.75,2.75,0));
	frames[2]->set_vbob(CAM_VIEWBOB_SLIDING);
	frames[2]->set_speed(PLAYER_SLIDE_MIN_SPEED,0,0);
	frames[2]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_SLIDE_END,ANIM_END_TYPE_DEFAULT_ANIM);
	//End maneuver
	frames[3]->set_bounds(Vec3(1.75,2.95,0));

	//=================================================
	//Dive through air duct tile [1]
	//=================================================
	tile_styles[0]->obst_tiles[1]->maneuvers[0] = new Maneuver(3);
	tile_styles[0]->obst_tiles[1]->maneuvers[0]->set_input(INPUT_SWIPE_UP);
	frames = tile_styles[0]->obst_tiles[1]->maneuvers[0]->keyframes;
	//Begin dive, aim towards center
	frames[0]->set_bounds(Vec3(0.75,0,0),Vec3(2.75,1,0));
	frames[0]->set_speed(PLAYER_RUN_SPEED-1.5f,FRAME_SPEED_CONST_TIME);
	frames[0]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,2.8,0),PLAYER_TURN_LERP_FACTOR);
	frames[0]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_DIVE,ANIM_END_TYPE_FREEZE);
	//Past wall, slowly re-aim towards forward
	frames[1]->set_bounds(Vec3(1.75,2.1,0));
	frames[1]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_DIVE_END,ANIM_END_TYPE_FREEZE);
	frames[1]->set_orient(FRAME_ORIENT_NONE,Vec3(1.75,3.5,0),0.1);
	frames[1]->set_speed(0.6,0,0);
	//End maneuver
	frames[2]->set_bounds(Vec3(1.75f,2.75,0));
	//=================================================
	//Run through doorways [2]
	//=================================================
		//No maneuver
	//=================================================
	//Run through elevator [3]
	//=================================================
		//No maneuver
	//=================================================
	//Vault through wall [4]
	//=================================================
	tile_styles[0]->obst_tiles[4]->maneuvers[0] = new Maneuver(3);
	tile_styles[0]->obst_tiles[4]->maneuvers[0]->set_input(INPUT_SWIPE_UP);
	frames = tile_styles[0]->obst_tiles[4]->maneuvers[0]->keyframes;
	//Begin speed vault, aim towards center
	frames[0]->set_bounds(Vec3(1.0,-0.5f,0),Vec3(2.5,0.5,0));
	frames[0]->set_speed(PLAYER_RUN_SPEED-2.5f,FRAME_SPEED_CONST_TIME);
	frames[0]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,0.95,0),PLAYER_TURN_LERP_FACTOR);
	frames[0]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_SPEED_VAULT,ANIM_END_TYPE_FREEZE);
	//Make player be at center
	frames[1]->set_bounds(Vec3(1.75,0.75,0));
	frames[1]->set_speed(PLAYER_RUN_SPEED - 4.0f,0,0);
	//End Maneuver
	frames[2]->set_bounds(Vec3(1.75,1.95,0));
	//=================================================
	//slide over TV [5]
	//=================================================
	tile_styles[0]->obst_tiles[5]->maneuvers[0] = new Maneuver(4);
	tile_styles[0]->obst_tiles[5]->maneuvers[0]->set_input(INPUT_SWIPE_UP);
	frames = tile_styles[0]->obst_tiles[5]->maneuvers[0]->keyframes;
	//Begin slide vault, aim towards center
	frames[0]->set_bounds(Vec3(1.0,-0.5f,0),Vec3(2.5,0.5,0));
	frames[0]->set_speed(PLAYER_RUN_SPEED-3.0f,FRAME_SPEED_CONST_TIME);
	frames[0]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,0.95,0),PLAYER_TURN_LERP_FACTOR);
	frames[0]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_VAULT_SLIDE,ANIM_END_TYPE_FREEZE);
	//Make player be at center
	frames[1]->set_bounds(Vec3(1.75,0.75,0));
	frames[1]->set_speed(PLAYER_RUN_SPEED - 2.0f,0,0);
	//Lower player
	frames[2]->set_bounds(Vec3(1.75,2.5,-0.3f));
	frames[2]->set_speed(PLAYER_RUN_SPEED - 2.0f,0,0);
	//End Maneuver
	frames[3]->set_bounds(Vec3(1.75,3.60,0));
	//=================================================
	//Kong over desks [6]
	//=================================================
	tile_styles[0]->obst_tiles[6]->maneuvers[0] = new Maneuver(4);
	tile_styles[0]->obst_tiles[6]->maneuvers[0]->set_input(INPUT_SWIPE_UP);
	frames = tile_styles[0]->obst_tiles[6]->maneuvers[0]->keyframes;
	//Begin kong, aim towards center
	frames[0]->set_bounds(Vec3(0.6,-0.25f,0),Vec3(2.9,0.75,0));
	frames[0]->set_speed(PLAYER_RUN_SPEED-1.5f,FRAME_SPEED_CONST_TIME);
	frames[0]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,3.5,0),PLAYER_TURN_LERP_FACTOR);
	frames[0]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_KONG,ANIM_END_TYPE_FREEZE);
	//Make player approach center
	frames[1]->set_bounds(Vec3(1.25,2,0),Vec3(2.25,2,0));
	frames[1]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,3.5,0),PLAYER_TURN_LERP_FACTOR);
	frames[1]->set_speed(PLAYER_RUN_SPEED,0,0);
	//Land at desk
	frames[2]->set_bounds(Vec3(1.75,2.75,0));
	frames[2]->set_speed(3.0,0,0);
	//End Maneuver
	frames[3]->set_bounds(Vec3(1.75,3.25,0));
	//=================================================
	//jump into elevator vent [7]
	//=================================================
	tile_styles[0]->obst_tiles[7]->maneuvers[0] = new Maneuver(5);
	tile_styles[0]->obst_tiles[7]->maneuvers[0]->set_input(INPUT_SWIPE_UP);
	frames = tile_styles[0]->obst_tiles[7]->maneuvers[0]->keyframes;
	//Align at center of elevator
	frames[0]->set_bounds(Vec3(1.0,-0.5f,0),Vec3(2.5,0.5,0));
	frames[0]->set_speed(0.8f,FRAME_SPEED_CONST_TIME);
	frames[0]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,1.25,0),PLAYER_TURN_LERP_FACTOR);
	frames[0]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_HIGH_UNDERBAR,ANIM_END_TYPE_FREEZE);
	//Move slowly forward as we climb up
	frames[1]->set_bounds(Vec3(1.75,0.51,0));
	frames[1]->set_speed(0.8f,0,0);
	//Move forward slightly faster as we enter vent
	frames[2]->set_bounds(Vec3(1.75,0.75,0));
	frames[2]->set_speed(0.8f,0,0);
	//Move forward as we roll
	frames[3]->set_bounds(Vec3(1.75,1.75,0));
	frames[3]->set_speed(0.8f,0,0);
	//End Maneuver
	frames[4]->set_bounds(Vec3(1.75,2.28,0));
	//=================================================
	//dash vault over decor [8]
	//=================================================
	tile_styles[0]->obst_tiles[8]->maneuvers[0] = new Maneuver(4);
	tile_styles[0]->obst_tiles[8]->maneuvers[0]->set_input(INPUT_SWIPE_UP);
	frames = tile_styles[0]->obst_tiles[8]->maneuvers[0]->keyframes;
	//Begin vault, aim towards center
	frames[0]->set_bounds(Vec3(0.5,0,0),Vec3(3,1,0));
	frames[0]->set_speed(PLAYER_RUN_SPEED-3.0f,FRAME_SPEED_CONST_TIME);
	frames[0]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,3.5,0),PLAYER_TURN_LERP_FACTOR);
	frames[0]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_DASH_VAULT,ANIM_END_TYPE_FREEZE);
	//Make player approach center
	frames[1]->set_bounds(Vec3(1.25,2,0),Vec3(2.25,2,0));
	frames[1]->set_orient(FRAME_ORIENT_NONE,Vec3(0,0,0),0.2);
	frames[1]->set_speed(PLAYER_RUN_SPEED,0,0);
	//Land on surface
	frames[2]->set_bounds(Vec3(1.5,2.75,-0.2f),Vec3(2.0,2.75,-0.2f));
	frames[1]->set_orient(FRAME_ORIENT_NONE,Vec3(0,0,0),0.2);
	frames[2]->set_speed(2.0,0,0);
	//End Maneuver
	frames[3]->set_bounds(Vec3(1.75,3.25,0));
	//=================================================
	//swing through bookshelf [9]
	//=================================================
	tile_styles[0]->obst_tiles[9]->maneuvers[0] = new Maneuver(4);
	tile_styles[0]->obst_tiles[9]->maneuvers[0]->set_input(INPUT_SWIPE_DOWN | INPUT_SWIPE_UP);
	frames = tile_styles[0]->obst_tiles[9]->maneuvers[0]->keyframes;
	//Run towards bookcase
	frames[0]->set_bounds(Vec3(1.25,0,0),Vec3(2.25,1,0));
	frames[0]->set_speed(3.0f,FRAME_SPEED_CONST_TIME);
	frames[0]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,3.5,0),PLAYER_TURN_LERP_FACTOR);
	frames[0]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_UNDERBAR,ANIM_END_TYPE_FREEZE);
	//Don't begin underbar until we are right up against wall
	frames[1]->set_bounds(Vec3(1.75,1.01,0));
	frames[1]->set_speed(3.0f,0,0);
	//Move player past bookcase
	frames[2]->set_bounds(Vec3(1.75,1.2,0));
	frames[2]->set_speed(3.0f,0,0);
	//End Maneuver
	frames[3]->set_bounds(Vec3(1.75,2.55,0));

	//======================================================================================================================
	//======================================================================================================================
	//======================================================================================================================


	//=================== Setting Up Window Models ====================
	window_styles[0] = new Exterior_Style();
	window_styles[0]->window_models = new Window_Model_Holder("models/windows/style0.stmf");
	window_styles[0]->int_window_models = new Interior_Window_Model_Holder("models/windows/style0_int.stmf");

	//Setting up the broken window models
	window_styles[0]->broken_in_window = new Skel_Model("models/windows/winbreak/winbreak_in.skmf");
	window_styles[0]->broken_out_window = new Skel_Model("models/windows/winbreak/winbreak_out.skmf");

	window_styles[0]->broken_in_window_skel_data = new Skeleton_Data("animations/winbreak/winbreak_in_skeleton.sksf");
	window_styles[0]->broken_out_window_skel_data = new Skeleton_Data("animations/winbreak/winbreak_out_skeleton.sksf");
	window_styles[0]->broken_in_window_skel_data->load_animation("animations/winbreak/winbreak_in.skaf");
	window_styles[0]->broken_out_window_skel_data->load_animation("animations/winbreak/winbreak_out.skaf");

	window_styles[0]->variants[0]->ext_diffuse_map = new Texture("textures/windows/variant0_diff.pkm",512,512);
	window_styles[0]->variants[0]->ext_misc_map = new Texture("textures/windows/variant0_misc.pkm",512,512);
	window_styles[0]->variants[0]->ext_normal_map = new Texture("textures/windows/variant0_nor.pkm",512,512);

	window_styles[0]->variants[0]->int_diffuse_map = new Texture("textures/windows/variant0_diff.pkm",512,512);
	window_styles[0]->variants[0]->int_misc_map = new Texture("textures/windows/variant0_misc.pkm",512,512);
	window_styles[0]->variants[0]->int_normal_map = new Texture("textures/windows/variant0_nor.pkm",512,512);

	//============= Setting up building to building traversals ==================
	//===========================================================================
	//This animation takes player down 1 floor
	bldg_travs[0] = new Traversal(8);
	bldg_travs[0]->set_input(INPUT_SWIPE_UP);
	frames = bldg_travs[0]->keyframes;
	//Distance between buildings: 15 m
	//Player is running to window
	frames[0]->set_bounds(Vec3(0.25,1.33,0),Vec3(3.25,2,0));//2
	frames[0]->set_speed(PLAYER_RUN_SPEED + 0.5f,FRAME_SPEED_CONST_TIME);
	frames[0]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,4,0),PLAYER_TURN_LERP_FACTOR);
	frames[0]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_RUN_PRE_JUMP,ANIM_END_TYPE_FREEZE);
	frames[0]->set_vbob(CAM_VIEWBOB_RUNNING);

	//Playing traversal animation
	frames[1]->set_bounds(Vec3(1.75,2.5,0));
	frames[1]->set_speed(PLAYER_RUN_SPEED - 1.5f,0,0);
	frames[1]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_TRAV_A,ANIM_END_TYPE_DEFAULT_ANIM);
	frames[1]->set_vbob(CAM_VIEWBOB_RUNNING);

	//Player breaks glass
	frames[2]->set_bounds(Vec3(1.75f,3.0f,0));
	frames[2]->set_speed(PLAYER_RUN_SPEED - 1.5f,0,0);
	frames[2]->set_specflag(FRAME_SPECFLAG_BREAKWINDOW_OUT);

	//Player jumps
	frames[3]->set_bounds(Vec3(1.75f,4.0f,0));
	frames[3]->set_speed(PLAYER_RUN_SPEED + 2.3f,0,0);
	frames[3]->set_lerp(FRAME_LERP_QUAD_TO_VERT,0);

	//Player is at apex of parabolic path
	frames[4]->set_bounds(Vec3(1.75f,8.5f,2.0f));
	frames[4]->set_speed(PLAYER_RUN_SPEED + 2.3f,0,0);
	frames[4]->set_lerp(FRAME_LERP_QUAD_FROM_VERT,0);

	//Player is breaking through the next window:
	frames[5]->set_bounds(Vec3(1.75f,18.0f,-WINDOW_TILE_SIZE + 1.0f));
	frames[5]->set_speed(PLAYER_RUN_SPEED + 2.3f,0,0);
	frames[5]->set_specflag(FRAME_SPECFLAG_BREAKWINDOW_IN);

	//Player lands
	frames[6]->set_bounds(Vec3(1.75f,20.0f,-WINDOW_TILE_SIZE));
	frames[6]->set_speed(0.8f,0,0);

	//Player player has finished landing, ready to run again
	frames[7]->set_bounds(Vec3(1.75f,20.65f,-WINDOW_TILE_SIZE));

	//===========================================================================
	//This animation takes player down 2 floors
	bldg_travs[1] = new Traversal(7);
	bldg_travs[1]->set_input(INPUT_SWIPE_UP);
	frames = bldg_travs[1]->keyframes;
	//Distance between buildings: 15 m
	//Player is running to window
	frames[0]->set_bounds(Vec3(0.25,0.66,0),Vec3(3.25,1.34,0));
	frames[0]->set_speed(PLAYER_RUN_SPEED + 0.5f,FRAME_SPEED_CONST_TIME);
	frames[0]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,4,0),PLAYER_TURN_LERP_FACTOR);
	frames[0]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_RUN_PRE_JUMP,ANIM_END_TYPE_FREEZE);
	frames[0]->set_vbob(CAM_VIEWBOB_RUNNING);

	//Playing traversal animation
	frames[1]->set_bounds(Vec3(1.75,2.5,0));
	frames[1]->set_speed(PLAYER_RUN_SPEED - 1.5f,0,0);
	frames[1]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_TRAV_B,ANIM_END_TYPE_DEFAULT_ANIM);
	frames[1]->set_vbob(CAM_VIEWBOB_RUNNING);

	//Player breaks glass
	frames[2]->set_bounds(Vec3(1.75f,3.0f,0));
	frames[2]->set_speed(PLAYER_RUN_SPEED - 1.5f,0,0);
	frames[2]->set_specflag(FRAME_SPECFLAG_BREAKWINDOW_OUT);

	//Player jumps
	frames[3]->set_bounds(Vec3(1.75f,4.0f,0));
	frames[3]->set_speed(PLAYER_RUN_SPEED + 1.8f,0,0);
	frames[3]->set_lerp(FRAME_LERP_QUAD_FROM_VERT,0);

	//This path has no parabolic vertex

	//Player is breaking through the next window:
	frames[4]->set_bounds(Vec3(1.75f,17.5f,-2*WINDOW_TILE_SIZE + 1.0f));
	frames[4]->set_speed(PLAYER_RUN_SPEED + 1.8f,0,0);
	frames[4]->set_specflag(FRAME_SPECFLAG_BREAKWINDOW_IN);

	//Player lands
	frames[5]->set_bounds(Vec3(1.75f,20.0f,-2*WINDOW_TILE_SIZE));
	frames[5]->set_speed(0.8f,0,0);

	//Player player has finished landing, ready to run again
	frames[6]->set_bounds(Vec3(1.75f,20.6f,-2*WINDOW_TILE_SIZE));

	//===========================================================================
	//This animation takes player down 3 floors
	bldg_travs[2] = new Traversal(8);
	bldg_travs[2]->set_input(INPUT_SWIPE_UP);
	frames = bldg_travs[2]->keyframes;
	//Player is running to window
	frames[0]->set_bounds(Vec3(0.25,0,0),Vec3(3.25,0.67,0));
	frames[0]->set_speed(PLAYER_RUN_SPEED+0.5f,FRAME_SPEED_CONST_TIME);
	frames[0]->set_orient(FRAME_ORIENT_CONSTANT,Vec3(1.75,4,0),1);
	frames[0]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_RUN_PRE_JUMP,ANIM_END_TYPE_FREEZE);
	frames[0]->set_vbob(CAM_VIEWBOB_RUNNING);

	//Playing traversal animation
	frames[1]->set_bounds(Vec3(1.75,2.5,0));
	frames[1]->set_speed(PLAYER_RUN_SPEED - 1.5f,0,0);
	frames[1]->set_anim(FRAME_ANIM_PLAY,PLAYER_ANIM_TRAV_C,ANIM_END_TYPE_DEFAULT_ANIM);
	frames[1]->set_vbob(CAM_VIEWBOB_RUNNING);

	//Player breaks glass
	frames[2]->set_bounds(Vec3(1.75f,3.0f,0));
	frames[2]->set_speed(PLAYER_RUN_SPEED - 1.5f,0,0);
	frames[2]->set_specflag(FRAME_SPECFLAG_BREAKWINDOW_OUT);

	//Player jumps
	frames[3]->set_bounds(Vec3(1.75f,4.0f,0));
	frames[3]->set_speed(PLAYER_RUN_SPEED + 1.6f,0,0);
	frames[3]->set_lerp(FRAME_LERP_QUAD_TO_VERT,0);

	//Player is at apex of parabolic path
	frames[4]->set_bounds(Vec3(1.75f,7.0f,0.8f));
	frames[4]->set_speed(PLAYER_RUN_SPEED + 1.6f,0,0);
	frames[4]->set_lerp(FRAME_LERP_QUAD_FROM_VERT,0);

	//Player is breaking through the next window:
	frames[5]->set_bounds(Vec3(1.75f,17.5f,-3*WINDOW_TILE_SIZE + 1.0f));
	frames[5]->set_speed(PLAYER_RUN_SPEED + 1.6f,0,0);
	frames[5]->set_specflag(FRAME_SPECFLAG_BREAKWINDOW_IN);

	//Player lands
	frames[6]->set_bounds(Vec3(1.75f,20.0f,-3*WINDOW_TILE_SIZE));
	frames[6]->set_speed(0.8f,0,0);

	//Player player has finished landing, ready to run again
	frames[7]->set_bounds(Vec3(1.75f,20.6f,-3*WINDOW_TILE_SIZE));
	//===================================================================================
}
Global_Tiles::~Global_Tiles()
{
	delete sky_cube_map;

	//============== Deleting Interior Style ============

	delete tile_styles[0]->empty_tile->model;
	delete tile_styles[0]->empty_tile;

	for(int i = 1; i < WALL_TYPE_COUNT; i++)
	{
		delete tile_styles[0]->wall_subtypes[i]->model;
		delete tile_styles[0]->wall_subtypes[i];
	}

	for(int i = 1; i < RAIL_TYPE_COUNT; i++)
	{
		if(tile_styles[0]->rail_subtypes[i])
		{
			delete tile_styles[0]->rail_subtypes[i]->model;
			delete tile_styles[0]->rail_subtypes[i];
		}
	}



	for(int i = 0; i < TILE_OBSTACLE_COUNT; i++)
	{
		delete tile_styles[0]->obst_tiles[i]->model;
		delete tile_styles[0]->obst_tiles[i];
	}

	delete tile_styles[0];
	//===================================================
	//============= Deleting Window Style ===============

	delete window_styles[0]->window_models;
	delete window_styles[0]->int_window_models;

	delete window_styles[0]->broken_in_window;
	delete window_styles[0]->broken_out_window;
	delete window_styles[0]->broken_in_window_skel_data;
	delete window_styles[0]->broken_out_window_skel_data;

	delete window_styles[0]->variants[0]->ext_diffuse_map;
	delete window_styles[0]->variants[0]->ext_misc_map;
	delete window_styles[0]->variants[0]->ext_normal_map;

	delete window_styles[0]->variants[0]->int_diffuse_map;
	delete window_styles[0]->variants[0]->int_misc_map;
	delete window_styles[0]->variants[0]->int_normal_map;

	//===================================================

	//Delete global traversals
	for(int i = 0; i < BUILDING_TRAV_COUNT; i++)
	{
		delete bldg_travs[i];
	}
}

void Global_Tiles::init_gl()
{
	if(!instance)
		return;
	instance->sky_cube_map->init_gl();
	instance->tile_styles[0]->variants[0]->init_gl();

	//==================== Style 0 models init gl ===================
	instance->tile_styles[0]->empty_tile->model->init_gl();

	for(int i = 1; i < WALL_TYPE_COUNT; i++)
	{
		instance->tile_styles[0]->wall_subtypes[i]->model->init_gl();
	}

	for(int i = 1; i < RAIL_TYPE_COUNT; i++)
	{
		if(RAIL_TYPE_IS_VALID[i])
		{
			instance->tile_styles[0]->rail_subtypes[i]->model->init_gl();
		}
	}


	for(int i = 0; i < TILE_OBSTACLE_COUNT; i++)
	{
		instance->tile_styles[0]->obst_tiles[i]->model->init_gl();
	}
	//===============================================================

	//================== Window Style 0 init gl =====================
	instance->window_styles[0]->variants[0]->init_gl();
	instance->window_styles[0]->window_models->init_gl();
	instance->window_styles[0]->int_window_models->init_gl();
	instance->window_styles[0]->broken_in_window->init_gl();
	instance->window_styles[0]->broken_out_window->init_gl();
	//===============================================================

}
void Global_Tiles::term_gl()
{
	if(!instance)
		return;
	instance->sky_cube_map->term_gl();
	instance->tile_styles[0]->variants[0]->term_gl();

	//==================== Style 0 models term gl ===================
	instance->tile_styles[0]->empty_tile->model->term_gl();

	for(int i = 1; i < WALL_TYPE_COUNT; i++)
	{
		instance->tile_styles[0]->wall_subtypes[i]->model->term_gl();
	}

	for(int i = 1; i < RAIL_TYPE_COUNT; i++)
	{
		if(RAIL_TYPE_IS_VALID[i])
		{
			instance->tile_styles[0]->rail_subtypes[i]->model->term_gl();
		}
	}

	for(int i = 0; i < TILE_OBSTACLE_COUNT; i++)
	{
		instance->tile_styles[0]->obst_tiles[i]->model->init_gl();
	}
	//===============================================================

	//================== Window Style 0 term gl =====================
	instance->window_styles[0]->variants[0]->term_gl();
	instance->window_styles[0]->window_models->term_gl();
	instance->window_styles[0]->int_window_models->term_gl();
	instance->window_styles[0]->broken_in_window->term_gl();
	instance->window_styles[0]->broken_out_window->term_gl();
	//===============================================================
}


Global_Tiles* Global_Tiles::instance = NULL;