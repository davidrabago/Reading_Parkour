//
// Created by F1 on 7/30/2016.
//

#ifndef ENGINE_JNI_INTERFACE_HPP
#define ENGINE_JNI_INTERFACE_HPP

#include "common.hpp"

//This class is a holder for all of the JNIEnv variables
class JNI_Interface
{
public:
	ANativeActivity* activity;
	JavaVM* java_vm;

	//The following variables are set as global references
	jclass activity_class;
	jobject activity_instance;
	jclass key_event_class;

	//Method references (these do not need to be stored as global references)
	jmethodID test_method;

	jmethodID show_ad_method;
	jmethodID hide_ad_method;

	jmethodID show_keyboard_method;
	jmethodID hide_keyboard_method;

	jmethodID get_key_event_char_method;
	jmethodID key_event_constructor;

	JNI_Interface(ANativeActivity* act);

	~JNI_Interface();

	bool get_jni_env(JNIEnv** env);

	bool detach_thread();

	//Shows ads
	void show_ad();
	//Hides ads
	void hide_ad();

	//Shows keyboard
	void show_keyboard();
	//Hides keyboard
	void hide_keyboard();

	//Method which calls the test java method
	void test_function();

	//Returns the character from a keyboard event
	char get_key_event_char(int event_type, int key_code, int meta_state);
};

#endif //ENGINE_JNI_INTERFACE_HPP