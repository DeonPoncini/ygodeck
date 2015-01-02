#include <jni.h>

#include "Utils.h"

#include <ygo/deck/c/CardSelector.h>

#ifdef __cplusplus
extern "C" {
#endif

#define JNI_SIGNATURE(_name) Java_net_sectorsoftware_ygo_deck_CardSelector_##_name

JNI_RETURN(jlong) JNI_SIGNATURE(init) (JNIEnv* env, jobject obj)
{
    return (jlong)(CARD_SEL(new)());
}

JNI_RETURN(void) JNI_SIGNATURE(delete) (JNIEnv* env, jobject obj, jlong p)
{
    CARD_SEL(delete)((CARD_SEL_THIS)p);
}

JNI_RETURN(jobject) JNI_SIGNATURE(execute) (JNIEnv* env, jobject obj, jlong p)
{
    int items;
    int i;
    char** list;
    list = CARD_SEL(execute)((CARD_SEL_THIS)p, &items);
    LOGE("Item length is: %d", items);

    jclass arrayListClass = (*env)->FindClass(env, "java/util/ArrayList");
    jmethodID ctor = (*env)->GetMethodID(env, arrayListClass, "<init>",
            "(I)V");
    jobject arrayList = (*env)->NewObject(env, arrayListClass, ctor, items);
    jmethodID addMethod = (*env)->GetMethodID(env, arrayListClass, "add",
            "(Ljava/lang/Object;)Z");

    for (i = 0; i < items; i++) {
        LOGE("Item: %s", list[i]);
        jstring s = c_string_to_java(env, list[i]);
        (*env)->CallBooleanMethod(env, arrayList, addMethod, s);
        (*env)->DeleteLocalRef(env, s);
    }

    CARD_SEL(execute_delete)(list, items);

    return arrayList;
}

JNI_RETURN(void) JNI_SIGNATURE(name)(JNIEnv* env, jobject obj, jlong p,
        jstring like)
{
    const char* nativeString = (*env)->GetStringUTFChars(env, like, 0);
    CARD_SEL(name)((CARD_SEL_THIS)p, nativeString);
    (*env)->ReleaseStringUTFChars(env, like, nativeString);
}

#ifdef __cplusplus
}
#endif
