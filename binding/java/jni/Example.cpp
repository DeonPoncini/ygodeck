#include <jni.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JNI_SIGNATURE(_name) Java_net_sectorsoftware_ygodeck_Example_##_name

JNIEXPORT jint JNI_SIGNATURE(increment) (JNIEnv* env, jobject obj, jint i)
{
    return i + 1;
}

#ifdef __cplusplus
}
#endif
