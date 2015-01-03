#include <jni.h>

#include "Utils.h"

#include <jniw/jniw.h>
#include <ygo/deck/c/DB.h>

#define JNI_SIGNATURE(_name) Java_net_sectorsoftware_ygo_deck_DB_##_name

JNI_RETURN(void) JNI_SIGNATURE(setPath)(JNIEnv* env, jobject obj, jstring path)
{
    const char* nativeString = (*env)->GetStringUTFChars(env, path, 0);
    DB_NAME(set_path)(nativeString);
    (*env)->ReleaseStringUTFChars(env, path, nativeString);
}

JNI_RETURN(jstring) JNI_SIGNATURE(getPath)(JNIEnv* env, jobject obj)
{
    char* path = DB_NAME(get_path)();
    jstring ret = c_string_to_java(env, path);
    DB_NAME(get_path_delete)(path);
    return ret;
}

