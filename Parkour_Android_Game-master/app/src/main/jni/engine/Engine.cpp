//
// Created by F1 on 3/23/2016.
//

#include "Engine.hpp"


Engine::Engine (struct android_app *droid_app)
{
	app = droid_app;

	app->userData = this;
	app->onAppCmd = Engine::handle_cmd;
	app->onInputEvent = Engine::handle_input;

	//Setting up accelerometer stuff
	sensor_manager = ASensorManager_getInstance();
	//Unused accelerometer sensor reference
	//accelerometer_sensor = ASensorManager_getDefaultSensor(sensor_manager,ASENSOR_TYPE_ACCELEROMETER);
	sensor_event_queue = ASensorManager_createEventQueue(sensor_manager, droid_app->looper, LOOPER_ID_USER, NULL, NULL);

	//Restore from a saved state
	if(droid_app->savedState != NULL)
	{
		//Really not sure when this is running, I'll add debug code so I can be informed when
		LOGE("NOT AN ERROR: notifying that previous saved state was not null, set state to prev state.\n");
		state = *(struct saved_state *) droid_app->savedState;
	}

	audio_engine = new Audio_Engine();

	jnii = new JNI_Interface(app->activity);

	game = new Game();
}

Engine::~Engine()
{
	if(audio_engine)
		delete audio_engine;
	if(jnii)
		delete jnii;
	if(game)
		delete game;

	term_data();
}


void Engine::handle_cmd (struct android_app *app, int32_t cmd)
{
	Engine *eng = (Engine *) app->userData;

	switch(cmd)
	{
		//Saving our current state
		case APP_CMD_SAVE_STATE:
			eng->app->savedState = malloc(sizeof(struct saved_state));
			*((struct saved_state *) eng->app->savedState) = (eng->state);

			eng->app->savedStateSize = sizeof(struct saved_state);
			break;
			//Window is being shown
		case APP_CMD_INIT_WINDOW:
			if(eng->app->window != NULL)
			{
				if(!eng->data_initialized)
				{
					LOGE("attempted to draw screen before loading data.\n");
					if(!eng->init_data())
					{
						return;
					}
				}
				if(eng->init_display() == -1)
				{
					LOGE("Init display returns -1");
					return;
				}
				eng->draw_frame();
			}
			break;
			//Window is being hidden
		case APP_CMD_TERM_WINDOW:
			eng->term_display();
			break;
			//App gained focus, start monitoring accelerometer
		case APP_CMD_GAINED_FOCUS:
			//if(eng->accelerometer_sensor != NULL)
			//{
			//	ASensorEventQueue_enableSensor(eng->sensor_event_queue,eng->accelerometer_sensor);
			//60 samples per second
			//	ASensorEventQueue_setEventRate(eng->sensor_event_queue,eng->accelerometer_sensor, (1000L/60)*1000);
			//}
			eng->audio_engine->start_audio();
			break;
			//App lost focus, stop monitoring acceleromter, and stop animating
		case APP_CMD_LOST_FOCUS:
			//if(eng->accelerometer_sensor != NULL)
			//{
			//	ASensorEventQueue_disableSensor(eng->sensor_event_queue,eng->accelerometer_sensor);
			//}
			eng->audio_engine->pause_audio();
			eng->animating = 0;
			eng->draw_frame();
			break;
		default:
			break;
	}
	return;
}

int32_t Engine::handle_input (struct android_app *app, AInputEvent *event)
{
	Engine *eng = (Engine *) app->userData;
	int type = AInputEvent_getType(event);

	if(type == AINPUT_EVENT_TYPE_MOTION)
	{
		int32_t motion_action = AMotionEvent_getAction(event);
		eng->animating = 1;

		int motion_type = motion_action & AMOTION_EVENT_ACTION_MASK;
		int event_type = 0;

		//if move: only update the fingers that have moved.
		int touch_count = AMotionEvent_getPointerCount(event);

		if(touch_count > 1)
		{
			//Only update the touch locations that have moved
			if(motion_type == AMOTION_EVENT_ACTION_MOVE)
			{
				float x = 0;
				float y = 0;
				int id = -1;
				event_type = INPUT_EVENT_ON_TOUCH_MOVE;
				for(size_t i = 0; i < touch_count; i++)
				{
					x = AMotionEvent_getX(event, i) / eng->width;
					y = AMotionEvent_getY(event, i) / eng->height;
					id = AMotionEvent_getPointerId(event,i);
					if(eng->state.inp_x[id] != x || eng->state.inp_y[id] != y)
					{
						eng->state.inp_x[id] = x;
						eng->state.inp_y[id] = y;
						//Update this touch
						if(eng->game)
						{
							eng->game->handle_input(x,y,event_type,id);
						}
					}
				}
				return 1;
			}
		}

		size_t pointer_index = (size_t) (motion_action & AMOTION_EVENT_ACTION_POINTER_INDEX_MASK) >> AMOTION_EVENT_ACTION_POINTER_INDEX_SHIFT;

		//for non-multi touch (just 1 finger), replace index in the next two with 0
		float touch_x = AMotionEvent_getX(event, pointer_index) / eng->width;
		float touch_y = AMotionEvent_getY(event, pointer_index) / eng->height;

		int pointer_id = AMotionEvent_getPointerId(event,pointer_index);

		eng->state.inp_x[pointer_id] = touch_x;
		eng->state.inp_y[pointer_id] = touch_y;

		switch(motion_type)
		{
			case AMOTION_EVENT_ACTION_DOWN:
			case AMOTION_EVENT_ACTION_POINTER_DOWN:
				event_type = INPUT_EVENT_ON_TOUCH_DOWN;
				break;
			case AMOTION_EVENT_ACTION_UP:
			case AMOTION_EVENT_ACTION_POINTER_UP:
				event_type = INPUT_EVENT_ON_TOUCH_RELEASE;
				break;
			case AMOTION_EVENT_ACTION_MOVE:
				event_type = INPUT_EVENT_ON_TOUCH_MOVE;
				break;
			default:
				break;
		}
		if(eng->game && event_type)
		{
			eng->game->handle_input(touch_x,touch_y,event_type,pointer_id);
		}
		return 1;
	}
	if(type == AINPUT_EVENT_TYPE_KEY)
	{
		int key_action = AKeyEvent_getAction(event);
		int key_code = AKeyEvent_getKeyCode(event);
		//Meta state holds info regarding whether shift was held, ctrl, alt, etc...
		int key_meta_state = AKeyEvent_getMetaState(event);

		//Don't handle hardware volume up / down
		if(key_code == AKEYCODE_VOLUME_UP || key_code == AKEYCODE_VOLUME_DOWN)
		{
			return 0;
		}

		if(key_action == AKEY_EVENT_ACTION_DOWN)
		{
			int event_type = 0;
			char event_key_char = 0;
			if(key_code == AKEYCODE_BACK)
			{
				event_type = INPUT_KEY_BACK;
			}

			//if(key_meta_state && AMETA_CAPS_LOCK_ON)//Checks for caps lock
			//if(key_meta_state && AMETA_SHIFT_ON)//Checks for shift
			event_key_char = eng->jnii->get_key_event_char(key_action,key_code,key_meta_state);

			//get key event returns null if backspace, and we want to catch backspace characters
			if(key_code == AKEYCODE_DEL)
			{
				event_key_char = '\b';
			}
			//Filtering out unwanted character through this array
			event_key_char = INPUT_CHAR_FILTER[event_key_char];
			if(event_key_char)
			{
				event_type = INPUT_KEY_KEYBOARD;
			}
			if(event_type)
			{
				eng->game->handle_key_input(event_type,event_key_char);
			}
		}
		return 1;
	}
	return 0;
}

int Engine::init_display ()
{
#ifdef DEBUG_MODE
	LOGI("LIFECYCLE: init display started.\n");
#endif
	//Setting up OPENGLES
	const EGLint config_attribs[] = {
	EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
	EGL_BLUE_SIZE, 8,
	EGL_GREEN_SIZE, 8,
	EGL_RED_SIZE, 8,
	EGL_DEPTH_SIZE, 24,
	EGL_NONE
	};
	EGLint w, h, num_configs;
	EGLConfig config;
	EGLSurface surface;
	EGLContext context;

	EGLDisplay display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	//Should we check for this?
	/*if(display == NULL)
	{
		LOGW("eglGetDisplay returned NULL");
		return -1;
	}*/
	eglInitialize(display, 0, 0);

	//Choosing first config that matches our criteria)
	eglChooseConfig(display, config_attribs, &config, 1, &num_configs);
	surface = eglCreateWindowSurface(display, config, app->window, NULL);

	const EGLint context_attribs[] = {EGL_CONTEXT_CLIENT_VERSION, 2, EGL_NONE};

	context = eglCreateContext(display, config, NULL, context_attribs);

	if(eglMakeCurrent(display, surface, surface, context) == EGL_FALSE)
	{
		//error, should return -1 or so
		LOGW("eglMakeCurrent Failed");
		return -1;
	}

	eglQuerySurface(display, surface, EGL_WIDTH, &w);
	eglQuerySurface(display, surface, EGL_HEIGHT, &h);

	egl_display = display;
	egl_context = context;
	egl_surface = surface;
	width = w;
	height = h;

	animating = 1;

	//====== GL Initialization code ======
	if(!init_gl())
		return -1;

	//Resume audio if it was paused previously
	if(audio_engine->sl_audio_player_interface != NULL)
		audio_engine->start_audio();

#ifdef DEBUG_MODE
	LOGI("LIFECYCLE: init display finished.\n");
#endif

	return 0;
}

void Engine::term_display ()
{
#ifdef DEBUG_MODE
	LOGI("LIFECYCLE: DISPLAY TERMINATED\n");
#endif

	//Need to run code that stops the executing code
	audio_engine->pause_audio();
	//====== GL termination code ======
	term_gl();
	//=================================
	if(egl_display != EGL_NO_DISPLAY)
	{
		eglMakeCurrent(egl_display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
		if(egl_context != EGL_NO_CONTEXT)
		{
			eglDestroyContext(egl_display, egl_context);
		}
		if(egl_surface != EGL_NO_SURFACE)
		{
			eglDestroySurface(egl_display, egl_surface);
		}
		eglTerminate(egl_display);
	}
	animating = 0;
	egl_display = EGL_NO_DISPLAY;
	egl_context = EGL_NO_CONTEXT;
	egl_surface = EGL_NO_SURFACE;

}

int Engine::init_gl ()
{
	//Init gl state
	//At this stage, all of the shaders have already been loaded.
	//glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_FASTEST);


	//Getting the default frame buffer
	glGetIntegerv(GL_FRAMEBUFFER_BINDING,&default_frame_buffer);


#ifdef RENDER_AT_LOWER_RES
	glGenFramebuffers(1,&frame_buffer);
	glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);

	glGenTextures(1,&render_texture);
	glBindTexture(GL_TEXTURE_2D,render_texture);
	//GLint texture_filtering = GL_LINEAR;
	GLint texture_filtering = GL_NEAREST;
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,texture_filtering);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,texture_filtering);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,render_texture,0);

	glGenRenderbuffers(1,&depth_render_buffer);
	glBindRenderbuffer(GL_RENDERBUFFER,depth_render_buffer);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,width,height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,depth_render_buffer);

	if(glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		LOGE("ERROR: Frame buffer status is not complete");
	}

	render_to_screen_shader->init_gl();

	//Restore the default frame buffer
	glBindFramebuffer(GL_FRAMEBUFFER,(GLuint)default_frame_buffer);
#endif

	glEnable(GL_CULL_FACE);
	//glDisable(GL_DEPTH_TEST);


	//enable blend function
	glEnable(GL_BLEND);
	//set blend function
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	game->init_gl();

	glViewport(0, 0, width, height);
	//glDepthRangef(0.0f,1.0f); useless line
	glClearColor(1, 1, 1, 1);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);

	gl_initialized = true;

	return 1;
}

void Engine::term_gl ()
{

#ifdef RENDER_AT_LOWER_RES
	glDeleteTextures(1,&render_texture);
	glDeleteRenderbuffers(1,&depth_render_buffer);
	glDeleteFramebuffers(1,&frame_buffer);

	render_to_screen_shader->term_gl();
#endif

	game->term_gl();
	gl_initialized = false;
}

int Engine::init_data ()
{

#ifdef RENDER_AT_LOWER_RES
	//Initializing the Engine's shader
	GLuint shader_ptypes[] =
	{
	Shader::PARAM_VERTICES,
	Shader::PARAM_VERT_UV1,
	Shader::PARAM_TEXTURE_DIFFUSE
	};
	const char *shader_pnames[] =
	{
	"vert_pos",
	"vert_uv",
	"tex_diff"
	};
	render_to_screen_shader = new Shader("shaders/render_to_screen.vert","shaders/render_to_screen.frag",shader_ptypes,shader_pnames,3);
	render_to_screen_mat = new Material();
	render_to_screen_mat->set_shader(render_to_screen_shader);
#endif

	LOGI("init_data...\n");
	if(!game->load_assets())
		return 0;
	data_initialized = true;
	return 1;
}

void Engine::term_data ()
{
#ifdef RENDER_AT_LOWER_RES
	delete render_to_screen_shader;
	delete render_to_screen_mat;
#endif
	game->unload_assets();
	data_initialized = false;
}

void Engine::first_frame()
{
	game->screen_width = width;
	game->screen_height = height;
	game->screen_ratio = ((float)width / (float)height);
	LOGE("Game aspect ratio: %f",game->screen_ratio);
	game->jnii = jnii;
	game->start();
	Audio_Engine::set_audio_listener(game->audio_listener);
}

//This is where we terminate any memory that was alllocated mid-game
void Engine::last_frame()
{
	game->finish();
}

void Engine::draw_frame()
{
	//Need to initialize data before the screen context has been created.
	if(!data_initialized)
	{
		return;
	}
#ifdef DEBUG_MODE
	static bool was_rendering = true;
	if(!egl_display)
	{
		if(was_rendering)
		{
			LOGI("LIFECYCLE: RENDER ABORTED, display not initialized\n");
			was_rendering = false;
		}
		return;
	}
	if(!was_rendering)
	{
		LOGI("LIFECYCLE: rendering...\n");
		was_rendering = true;
	}
#endif

	if(!egl_display)
	{
		return;
	}

	//Don't try rendering until update has executed first frame logic
	if(is_first_frame)
	{
		return;
	}
	float t = Time::time();
	//Evaluating global time variables
	Time::fdelta_time = t - Time::last_frame_time;
	//If way too much time has elapsed since last frame, lerping things will get messed up, so make delta_time be 0
	if(Time::fdelta_time >= 1.0f/5.0f)
		Time::fdelta_time = 0.0f;//(setting delta time to 0 may cause issues)
	Time::last_frame_time = t;


#ifdef RENDER_AT_LOWER_RES
	float resolution_scale = game->engine_render_resolution_scale;
	glBindFramebuffer(GL_FRAMEBUFFER,frame_buffer);
	glViewport(0,0,(int)(width * resolution_scale),(int)(height * resolution_scale));
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
#endif

	game->render();

	frame_count++;

	//======================================== FPS Rendering ================================================
	static float frame_count_start_time = t;
	static int frame_count_60 = 0;//set to 0 every 60 frames
	static float fps = 0;

	//Count average fps over 60 frames
	if(frame_count_60 >= 60)
	{
		fps = frame_count_60 / (t - frame_count_start_time);
		frame_count_start_time = t;
		frame_count_60 = -1;
	}
	frame_count_60++;
	//Drawing FPS
	//Disabled drawing FPS
//	char fps_str[20];
//	snprintf(fps_str,20,"FPS: %.2f",fps);
//	UI_Text::draw_text(fps_str,Vec3(width * 0.1f, height * 0.4f,0.5f),Vec3::ZERO(),100,Vec3(1,1,1),Vec3::ZERO(),1,false,
//		game->camera->ortho_proj_m);
	//============================================================================================

	//============================= Rendering the smaller res texture at full rez =============================

#ifdef RENDER_AT_LOWER_RES
	glBindFramebuffer(GL_FRAMEBUFFER,default_frame_buffer);
	glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
	glViewport(0,0,width,height);

	const float quad_verts[] =
	{
	-1.0f,1.0f,0.3f,
	-1.0f,-1.0f,0.3f,
	1.0f,-1.0f,0.3f,
	1.0f,-1.0f,0.3f,
	1.0f,1.0f,0.3f,
	-1.0f,1.0f,0.3f
	};

	const float quad_uvs[] =
	{
		0, resolution_scale,
		0,0,
		resolution_scale,0,
		resolution_scale,0,
		resolution_scale,resolution_scale,
		0,resolution_scale
	};

	render_to_screen_mat->bind_material();
	render_to_screen_mat->bind_value(Shader::PARAM_VERTICES,(void*) quad_verts);
	render_to_screen_mat->bind_value(Shader::PARAM_VERT_UV1,(void*) quad_uvs);
	//Constructing a null texture whose texture gl_id is render_texture
	Texture tex;
	tex.gl_id = render_texture;
	render_to_screen_mat->bind_value(Shader::PARAM_TEXTURE_DIFFUSE,(void*) &tex);
	glDrawArrays(GL_TRIANGLES, 0, 6);
#endif


	eglSwapBuffers(egl_display, egl_surface);

}

void Engine::update()
{
	//Need to initialize data before the screen context has been created.
	if(!data_initialized)
	{
		if(!init_data())
			return;
	}

	if(!egl_display)
	{
		return;
	}

	if(is_first_frame)
	{
		first_frame();
		is_first_frame = false;
	}

	float t = Time::time();
	//Evaluating global time variables
	Time::udelta_time = t - Time::last_update_time;
	//If way too much time has elapsed since last frame, lerping things will get messed up, so make delta_time be 0
	if(Time::udelta_time >= 1.0f/5.0f)
		Time::udelta_time = 0.0f;//(setting delta time to 0 may cause issues)
	Time::last_update_time = t;

	game->update();

	update_count++;

	//======================================== Updates per second calculations ================================================
	/*static float update_count_start_time = Time::time();
	static int update_count_60 = 0;//set to 0 every 60 frames
	static float ups = 0;

	//Count average fps over 60 frames
	if(update_count_60 >= 60)
	{
		ups = frame_count / (Time::time() - update_count_start_time);
		update_count_60 = -1;
	}
	update_count_60++;*/
	//============================================================================================
}