#include <jni.h>

#ifdef ANDROID
#include <android/log.h>
#else
#include <stdio.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define LOG_TAG "ygodeck"

#ifdef ANDROID
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)
#else
#define LOGE(...) printf(__VA_ARGS__)
#endif

#define JNI_RETURN(name) JNIEXPORT name JNICALL

jstring c_string_to_java(JNIEnv* env, char* str);

#ifdef __cplusplus
}
#endif
