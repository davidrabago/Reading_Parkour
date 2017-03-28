//
// Created by F1 on 3/23/2016.
//

#ifndef ENGINE_ENGINE_H
#define ENGINE_ENGINE_H

#include "common.hpp"
#include "misc.hpp"
#include "sound/Audio_Engine.hpp"
#include "File_Utils.hpp"
#include "graphics/GL_Utils.hpp"
#include "graphics/Camera.hpp"
#include "../entities/Player.hpp"
#include "graphics/Shader.hpp"
#include "graphics/Material.hpp"
#include "graphics/Skel_Model.hpp"
#include "graphics/Skeleton.hpp"
#include "graphics/Texture.hpp"
#include "graphics/UI.hpp"
#include "graphics/Static_Model.hpp"
#include "graphics/Cube_Map.hpp"
#include "graphics/Skybox.hpp"
#include "JNI_Interface.hpp"
#include "../Game.hpp"


// Saved State Data
struct saved_state
{
	//Input coordinates
	float inp_x[10];
	float inp_y[10];
};

class Engine
{
public:
	Engine(struct android_app *app);
	~Engine();

	static void handle_cmd(struct android_app *app, int32_t cmd);
	static int32_t handle_input(struct android_app *app, AInputEvent *event);

	JNI_Interface* jnii;

	void term_display();
	int init_display();

	Audio_Engine* audio_engine;

	int init_gl();
	void term_gl();
	bool gl_initialized = false;


	int init_data();
	void term_data();
	bool data_initialized = false;

	void draw_frame();
	uint frame_count = 0;
	void update();
	uint update_count = 0;

	bool is_first_frame = true;

	void first_frame();
	void last_frame();

	EGLDisplay egl_display = 0;
	EGLSurface egl_surface = 0;
	EGLContext egl_context = 0;

	//FBO downscaled rendering member variables
	GLint default_frame_buffer = 0;
	GLuint frame_buffer;
	GLuint depth_render_buffer;
	GLuint render_texture;

	Shader *render_to_screen_shader = NULL;
	Material *render_to_screen_mat = NULL;


	struct android_app *app;

	ASensorManager *sensor_manager;
	//Unused accelerometer reference
	//const ASensor *accelerometer_sensor;
	ASensorEventQueue *sensor_event_queue;

	int32_t width;
	int32_t height;
	struct saved_state state;
	//struct saved_state* state;
	int animating;

	Game* game;
};


#endif //ENGINE_ENGINE_H
