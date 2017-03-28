//
// Created by F1 on 10/12/2016.
//

#ifndef PARKOUR_ENGINE_DEFS_HPP
#define PARKOUR_ENGINE_DEFS_HPP

//This is the internal data value for opengl to recognize ETC1 compressed textures
#define ETC1_RGB8 0x8D64

#define SND_AUDIO_CHANNELS 8
#define SND_AUDIO_BUFFER_SIZE 512
#define SND_AUDIO_SAMPLE_RATE 44100

#define ASPECT_16_9_LANDSCAPE 1.7778f
#define ASPECT_16_9_PORTRAIT 0.5625f


//Enable this to render the game at a lower resolution and upscale it to the game's res
#define RENDER_AT_LOWER_RES

//=========== Texture Defs =============
#define TEXTURE_FORMAT_NONE 0
#define TEXTURE_FORMAT_ETC1 1
#define TEXTURE_FORMAT_RGBA_8888 2

//============ Skeletal Animation Defs ==========
#define ANIM_END_TYPE_ROOT_POSE 0
#define ANIM_END_TYPE_FREEZE 1
#define ANIM_END_TYPE_LOOP 2
#define ANIM_END_TYPE_DEFAULT_ANIM 3

#define ANIM_FPS_60 (1.0f/60.0f)

//The maximum number of frames that can be skipped by a skeleton in a single update call (used when update fps is low but animation fps is high)
#define ANIM_MAX_SKIPPABLE_FRAMES 5

//============ Sound Engine Defs ==============
#define MAX_SOUND_SOURCES 32

#define SOUND_END_TYPE_STOP 0
#define SOUND_END_TYPE_LOOP 1

#define SOUND_PRIORITY_AMBIENCE 0
#define SOUND_PRIORITY_PLAYER 1
#define SOUND_PRIORITY_PHYSICS 2
#define SOUND_PRIORITY_MISC 3

#endif //PARKOUR_ENGINE_DEFS_HPP
