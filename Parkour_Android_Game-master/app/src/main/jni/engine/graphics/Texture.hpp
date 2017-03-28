//
// Created by F1 on 6/5/2016.
//

#ifndef ENGINE_TEXTURE_H
#define ENGINE_TEXTURE_H

#include "../common.hpp"
#include "../File_Utils.hpp"

class Texture
{
public:
	const char* raw_data;
	GLuint gl_id = 0;
	int texture_format = TEXTURE_FORMAT_NONE;
	int raw_data_length = 0;
	int width,height = 0;
	int init_gl();
	void term_gl();

	//Used for constructing an empty texture
	//This is used as a container to be able to pass along raw gl textures by writing the texture's gl_id
	Texture(){};
	Texture(const char* filepath,int w, int h);
	~Texture();
private:
	int load(const char* filepath,int w, int h);
	void unload();
};

#endif //ENGINE_TEXTURE_H
