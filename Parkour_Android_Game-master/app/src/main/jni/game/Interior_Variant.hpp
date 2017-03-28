//
// Created by F1 on 9/12/2016.
//

#ifndef PARKOUR_INTERIOR_VARIANT_HPP
#define PARKOUR_INTERIOR_VARIANT_HPP

#include "../engine/graphics/Material.hpp"
#include "../engine/graphics/Texture.hpp"
#include "../engine/graphics/Cube_Map.hpp"

//This class is responsible for handling the materials / textures of each variety of interior tile style
class Interior_Variant
{
public:
	//All variants use the same shader, store as static within the class
	static Shader* shader;
	static bool shader_gl_inited;

	Material* mat = NULL;

	Texture* diffuse_map = NULL;
	Texture* normal_map = NULL;
	Texture* light_map = NULL;

	Cube_Map* ref_cube_map = NULL;


	//Three components to misc map:
	//R: cubemap reflectiveness
	//G: unassigned
	//B: unassigned

	//Previously planned (but scrapped)
	//G: specularity (try using lightmap value as specularity instead to free this for palette colors)
	//B: transparency
	Texture* misc_map = NULL;


	static int init_static_data(const char *vsrc, const char *fsrc,const GLuint *ptypes, const char **pnames, uint pcount)
	{
		if(!shader)
		{
			shader = new Shader(vsrc,fsrc,ptypes,pnames,pcount);
		}
		return 1;
	}
	static void term_static_data()
	{
		delete shader;
		shader = NULL;
	}


	Interior_Variant()
	{
		mat = new Material();
		mat->set_shader(shader);
	}

	~Interior_Variant()
	{
		delete diffuse_map;
		delete normal_map;
		delete misc_map;
		delete light_map;
		delete ref_cube_map;
		delete mat;
	}


	//Binds the material and any defined textures
	int bind_variant()
	{
		mat->bind_material();

		if(diffuse_map)
			mat->bind_value(Shader::PARAM_TEXTURE_DIFFUSE,(void*) diffuse_map);
		if(normal_map)
			mat->bind_value(Shader::PARAM_TEXTURE_NORMAL,(void*) normal_map);
		if(light_map)
			mat->bind_value(Shader::PARAM_TEXTURE_LIGHTMAP,(void*) light_map);
		if(misc_map)
			mat->bind_value(Shader::PARAM_TEXTURE_MISC,(void*) misc_map);
		if(ref_cube_map)
			mat->bind_value(Shader::PARAM_CUBE_MAP,(void*) ref_cube_map);

		//TODO: pass in all material data here (not including any mesh data)
		//this includes generated color palettes and cubemaps
		return 1;
	};

	//TODO: generate variant.
	//This method makes a new interior color palette, ready to be bound by the bind_variant method

	int init_gl()
	{
		if(!shader_gl_inited)
		{
			shader->init_gl();
			shader_gl_inited = true;
		}

		if(diffuse_map)
			diffuse_map->init_gl();
		if(normal_map)
			normal_map->init_gl();
		if(light_map)
			light_map->init_gl();
		if(misc_map)
			misc_map->init_gl();
		if(ref_cube_map)
			ref_cube_map->init_gl();
		return 1;
	}
	void term_gl()
	{
		if(shader && shader_gl_inited)
		{
			shader->term_gl();
			shader_gl_inited = false;
		}

		if(diffuse_map)
			diffuse_map->term_gl();
		if(normal_map)
			normal_map->term_gl();
		if(light_map)
			light_map->term_gl();
		if(misc_map)
			misc_map->term_gl();
		if(ref_cube_map)
			ref_cube_map->term_gl();
	}
};


//=======================================================================================================
//Very similar class as Interior_Variant made for window assets
//This class is responsible for handling the materials / textures of each variety of interior tile style
//=======================================================================================================

class Exterior_Variant
{
public:
	//All variants use the same shaders, store as static within the class
	//Exterior window material and shader
	//Shader for outer windows
	static Shader* ext_shader;
	//Shader for inner windows
	static Shader* int_shader;

	//Used for breaking into a building
	static Shader* ext_skel_shader;
	//Used for breaking out of a building
	static Shader* int_skel_shader;

	static bool shader_gl_inited;

	//Material for outer windows
	Material* ext_mat = NULL;
	//Material for inner windows
	Material* int_mat = NULL;

	//Material for skeletal outer windows
	Material* ext_skel_mat = NULL;
	//Material for skeletal inner windows
	Material* int_skel_mat = NULL;

	Texture* int_diffuse_map;
	Texture* int_misc_map;
	Texture* int_normal_map;

	Texture* ext_diffuse_map;
	Texture* ext_misc_map;
	Texture* ext_normal_map;

	//Three components to misc map:
	//R: cubemap reflectiveness
	//G: transparency (for inner windows only)
	//B: unassigned

	//Previously planned (but scrapped)
	//G: specularity (try using lightmap value as specularity instead to free this for palette colors)
	//B: transparency

	static int init_static_int_shader(const char *vsrc, const char *fsrc,const GLuint *ptypes, const char **pnames, uint pcount)
	{
		if(!int_shader)
		{
			int_shader = new Shader(vsrc,fsrc,ptypes,pnames,pcount);
		}
		return 1;
	}

	static int init_static_ext_shader(const char *vsrc, const char *fsrc,const GLuint *ptypes, const char **pnames, uint pcount)
	{
		if(!ext_shader)
		{
			ext_shader = new Shader(vsrc,fsrc,ptypes,pnames,pcount);
		}
		return 1;
	}

	static int init_static_int_skel_shader(const char *vsrc, const char *fsrc,const GLuint *ptypes, const char **pnames, uint pcount)
	{
		if(!int_skel_shader)
		{
			int_skel_shader = new Shader(vsrc,fsrc,ptypes,pnames,pcount);
		}
		return 1;
	}
	static int init_static_ext_skel_shader(const char *vsrc, const char *fsrc,const GLuint *ptypes, const char **pnames, uint pcount)
	{
		if(!ext_skel_shader)
		{
			ext_skel_shader = new Shader(vsrc,fsrc,ptypes,pnames,pcount);
		}
		return 1;
	}

	static void term_static_data()
	{
		delete int_shader;
		delete ext_shader;
		delete int_skel_shader;
		delete ext_skel_shader;
		int_shader = NULL;
		ext_shader = NULL;
		int_skel_shader = NULL;
		ext_skel_shader = NULL;
	}


	Exterior_Variant()
	{
		int_mat = new Material();
		int_mat->set_shader(int_shader);
		ext_mat = new Material();
		ext_mat->set_shader(ext_shader);
		int_skel_mat = new Material();
		int_skel_mat->set_shader(int_skel_shader);
		ext_skel_mat = new Material();
		ext_skel_mat->set_shader(ext_skel_shader);
	}

	~Exterior_Variant()
	{
		delete int_mat;
		delete ext_mat;
		delete int_skel_mat;
		delete ext_skel_mat;
		delete int_diffuse_map;
		delete int_normal_map;
		delete int_misc_map;
		delete ext_diffuse_map;
		delete ext_normal_map;
		delete ext_misc_map;
	}


	//Binds the interior material and any defined interior textures
	int bind_variant_int()
	{
		int_mat->bind_material();

		if(int_diffuse_map)
			int_mat->bind_value(Shader::PARAM_TEXTURE_DIFFUSE,(void*) int_diffuse_map);
		if(int_normal_map)
			int_mat->bind_value(Shader::PARAM_TEXTURE_NORMAL,(void*) int_normal_map);
		if(int_misc_map)
			int_mat->bind_value(Shader::PARAM_TEXTURE_MISC,(void*) int_misc_map);
		return 1;
	};

	int bind_variant_ext()
	{
		ext_mat->bind_material();

		if(ext_diffuse_map)
			ext_mat->bind_value(Shader::PARAM_TEXTURE_DIFFUSE,(void*) ext_diffuse_map);
		if(ext_normal_map)
			ext_mat->bind_value(Shader::PARAM_TEXTURE_NORMAL,(void*) ext_normal_map);
		if(ext_misc_map)
			ext_mat->bind_value(Shader::PARAM_TEXTURE_MISC,(void*) ext_misc_map);
		return 1;
	};

	int bind_variant_int_skel()
	{
		int_skel_mat->bind_material();

		if(int_diffuse_map)
			int_skel_mat->bind_value(Shader::PARAM_TEXTURE_DIFFUSE,(void*) int_diffuse_map);
		if(int_normal_map)
			int_skel_mat->bind_value(Shader::PARAM_TEXTURE_NORMAL,(void*) int_normal_map);
		if(int_misc_map)
			int_skel_mat->bind_value(Shader::PARAM_TEXTURE_MISC,(void*) int_misc_map);
		return 1;
	};

	int bind_variant_ext_skel()
	{
		ext_skel_mat->bind_material();

		if(ext_diffuse_map)
			ext_skel_mat->bind_value(Shader::PARAM_TEXTURE_DIFFUSE,(void*) ext_diffuse_map);
		if(ext_normal_map)
			ext_skel_mat->bind_value(Shader::PARAM_TEXTURE_NORMAL,(void*) ext_normal_map);
		if(ext_misc_map)
			ext_skel_mat->bind_value(Shader::PARAM_TEXTURE_MISC,(void*) ext_misc_map);
		return 1;
	};

	int init_gl()
	{
		if(!shader_gl_inited)
		{
			if(int_shader)
				int_shader->init_gl();
			if(ext_shader)
				ext_shader->init_gl();
			if(int_skel_shader)
				int_skel_shader->init_gl();
			if(ext_skel_shader)
				ext_skel_shader->init_gl();
			shader_gl_inited = true;
		}

		if(int_diffuse_map)
			int_diffuse_map->init_gl();
		if(int_normal_map)
			int_normal_map->init_gl();
		if(int_misc_map)
			int_misc_map->init_gl();

		if(ext_diffuse_map)
			ext_diffuse_map->init_gl();
		if(ext_normal_map)
			ext_normal_map->init_gl();
		if(ext_misc_map)
			ext_misc_map->init_gl();
		return 1;
	}
	void term_gl()
	{
		if(shader_gl_inited)
		{
			if(int_shader)
				int_shader->term_gl();
			if(ext_shader)
				ext_shader->term_gl();
			if(int_skel_shader)
				int_skel_shader->term_gl();
			if(ext_skel_shader)
				ext_skel_shader->term_gl();
			shader_gl_inited = false;
		}

		if(int_diffuse_map)
			int_diffuse_map->term_gl();
		if(int_normal_map)
			int_normal_map->term_gl();
		if(int_misc_map)
			int_misc_map->term_gl();

		if(ext_diffuse_map)
			ext_diffuse_map->term_gl();
		if(ext_normal_map)
			ext_normal_map->term_gl();
		if(ext_misc_map)
			ext_misc_map->term_gl();
	}
};


#endif //PARKOUR_INTERIOR_VARIANT_HPP
