//
// Created by F1 on 7/30/2016.
//

#include "JNI_Interface.hpp"

JNI_Interface::JNI_Interface(ANativeActivity* act)
{
	activity = act;
	java_vm = activity->vm;

	JNIEnv* jni;

	bool attached_jni_thread = get_jni_env(&jni);

	jclass local_activity_class = jni->GetObjectClass(activity->clazz);
	activity_class = (jclass) jni->NewGlobalRef(local_activity_class);

	if(!activity_class)
	{
		LOGE("Error: Failed to get native activity class");
	}
	else
		LOGI("Found native activity class");


	test_method = jni->GetMethodID(activity_class,"test","(II)Z");

	show_ad_method = jni->GetMethodID(activity_class,"showAd","()V");
	hide_ad_method = jni->GetMethodID(activity_class,"hideAd","()V");

	show_keyboard_method = jni->GetMethodID(activity_class,"showKeyboard","()V");
	hide_keyboard_method = jni->GetMethodID(activity_class,"hideKeyboard","()V");

	jclass local_key_event_class = jni->FindClass("android/view/KeyEvent");
	key_event_class = (jclass) jni->NewGlobalRef(local_key_event_class);

	get_key_event_char_method = jni->GetMethodID(key_event_class,"getUnicodeChar","(I)I");
	key_event_constructor = jni->GetMethodID(key_event_class,"<init>","(II)V");

	jobject local_activity_instance = activity->clazz;
	activity_instance = jni->NewGlobalRef(local_activity_instance);

	if(attached_jni_thread)
	{
		detach_thread();
	}
}

JNI_Interface::~JNI_Interface()
{
	JNIEnv* jni;

	bool attached_jni_thread = get_jni_env(&jni);

	jni->DeleteGlobalRef(activity_instance);
	jni->DeleteGlobalRef(activity_class);
	jni->DeleteGlobalRef(key_event_class);

	if(attached_jni_thread)
	{
		detach_thread();
	}
}

bool JNI_Interface::get_jni_env(JNIEnv** env)
{
	*env = NULL;
	jint result = (*java_vm).GetEnv((void**) env,JNI_VERSION_1_6);
	if(result == JNI_EDETACHED)
	{
		return ((*java_vm).AttachCurrentThread(env,NULL) == JNI_OK);
	}
	else if(result == JNI_EVERSION)
		{
			LOGE("Error: Failed to invoke JNI instance. Unsupported JNI version.");
		}
	return false;
}

bool JNI_Interface::detach_thread()
{
	java_vm->DetachCurrentThread();
	return true;
}

//Shows ads
void JNI_Interface::show_ad()
{
	JNIEnv* jni;

	bool attached_jni_thread = get_jni_env(&jni);
	jni->CallVoidMethod(activity_instance,show_ad_method);
	if(attached_jni_thread)
	{
		detach_thread();
	}
}
//Hides ads
void JNI_Interface::hide_ad()
{
	JNIEnv* jni;

	bool attached_jni_thread = get_jni_env(&jni);
	jni->CallVoidMethod(activity_instance,hide_ad_method);
	if(attached_jni_thread)
	{
		detach_thread();
	}
}

//Shows keyboard
void JNI_Interface::show_keyboard()
{
	JNIEnv* jni;

	bool attached_jni_thread = get_jni_env(&jni);
	jni->CallVoidMethod(activity_instance,show_keyboard_method);
	if(attached_jni_thread)
	{
		detach_thread();
	}
}
//Hides keyboard
void JNI_Interface::hide_keyboard()
{
	JNIEnv* jni;

	bool attached_jni_thread = get_jni_env(&jni);
	jni->CallVoidMethod(activity_instance,hide_keyboard_method);
	if(attached_jni_thread)
	{
		detach_thread();
	}
}

//Returns the character that an input event typed
char JNI_Interface::get_key_event_char(int event_type, int key_code, int meta_state)
{
	JNIEnv* jni;
	bool attached_jni_thread = get_jni_env(&jni);

	int unicode_char = 0;

	jobject key_event_obj = jni->NewObject(key_event_class, key_event_constructor, event_type, key_code);
	unicode_char = jni->CallIntMethod(key_event_obj,get_key_event_char_method,meta_state);

	if(attached_jni_thread)
	{
		detach_thread();
	}

	return (char) unicode_char;
}


//Method which calls the test java method
void JNI_Interface::test_function()
{
	JNIEnv* jni;

	bool attached_jni_thread = get_jni_env(&jni);

	jvalue java_args[2];
	java_args[0].i = 2;
	java_args[1].i = 3;

	jboolean result = jni->CallBooleanMethodA(activity_instance,test_method,java_args);
	LOGE("2 == 3? %d",result);

	java_args[0].i = 49;
	java_args[1].i = 49;

	result = jni->CallBooleanMethodA(activity_instance,test_method,java_args);
	LOGE("49 == 49? %d",result);

	if(attached_jni_thread)
	{
		detach_thread();
	}
}