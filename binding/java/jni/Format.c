#include <jni.h>

#include <jniw/jniw.h>
#include <ygo/deck/c/Format.h>

#define JNI_SIGNATURE(_name) Java_net_sectorsoftware_ygo_deck_Format_##_name

JNI_RETURN(jlong) JNI_SIGNATURE(init) (JNIEnv* env, jobject obj, jint format, jstring formatDate)
{
    const char* str = (*env)->GetStringUTFChars(env, formatDate, 0);
    jlong thiz = (jlong) FORMAT_NAME(new)((ygo_data_Format) format, str);
    (*env)->ReleaseStringUTFChars(env, formatDate, str);
    return thiz;
}

JNI_RETURN(void) JNI_SIGNATURE(delete) (JNIEnv* env, jobject obj, jlong p)
{
    FORMAT_NAME(delete)((FORMAT_THIS) p);
}

JNI_RETURN(jint) JNI_SIGNATURE(format) (JNIEnv* env, jobject obj, jlong p)
{
    return (jint) FORMAT_NAME(format)((FORMAT_THIS) p);
}

JNI_RETURN(jstring) JNI_SIGNATURE(formatDate) (JNIEnv* env, jobject obj,
        jlong p)
{
    char* d = FORMAT_NAME(formatDate)((FORMAT_THIS) p);
    jstring ret = jniw_to_jstring(env, d);
    FORMAT_NAME(delete_formatDate)(d);
    return ret;
}

JNI_RETURN(jint) JNI_SIGNATURE(cardCount) (JNIEnv* env, jobject obj, jlong p,
        jstring card)
{
    const char* str = (*env)->GetStringUTFChars(env, card, 0);
    jint ret = (jint) FORMAT_NAME(cardCount)((FORMAT_THIS) p, str);
    (*env)->ReleaseStringUTFChars(env, card, str);
    return ret;
}

JNI_RETURN(jobject) JNI_SIGNATURE(formatDates) (JNIEnv* env, jobject obj)
{
    int count;
    char** ds = FORMAT_NAME(formatDates)(&count);

    jobject arrayList = jniw_arraylist_create_r(env, count);
    jclass arrayListClass = (*env)->FindClass(env, "java/util/ArrayList");
    jmethodID addMethod = (*env)->GetMethodID(env, arrayListClass, "add",
            "(Ljava/lang/Object;)Z");

    for (int i = 0; i < count; i++) {
        jstring s = jniw_to_jstring(env, ds[i]);
        (*env)->CallBooleanMethod(env, arrayList, addMethod, s);
        (*env)->DeleteLocalRef(env, s);
    }

    FORMAT_NAME(delete_formatDates)(ds, count);

    return arrayList;
}

JNI_RETURN(jobject) JNI_SIGNATURE(formats) (JNIEnv* env, jobject obj)
{
    int count;
    char** ds = FORMAT_NAME(formats)(&count);

    jobject arrayList = jniw_arraylist_create_r(env, count);
    jclass arrayListClass = (*env)->FindClass(env, "java/util/ArrayList");
    jmethodID addMethod = (*env)->GetMethodID(env, arrayListClass, "add",
            "(Ljava/lang/Object;)Z");

    for (int i = 0; i < count; i++) {
        jstring s = jniw_to_jstring(env, ds[i]);
        (*env)->CallBooleanMethod(env, arrayList, addMethod, s);
        (*env)->DeleteLocalRef(env, s);
    }

    FORMAT_NAME(delete_formats)(ds, count);

    return arrayList;
}
