#include <jni.h>

#include <jniw/jniw.h>
#include <ygo/deck/c/User.h>

#define JNI_SIGNATURE(_name) Java_net_sectorsoftware_ygo_deck_User_##_name

JNI_RETURN(jlong) JNI_SIGNATURE(init) (JNIEnv* env, jobject obj,
        jstring name, jboolean create)
{
    const char* nativeString = (*env)->GetStringUTFChars(env, name, 0);
    jlong thiz;
    if (create) {
        thiz = (jlong)USER_NAME(new_create)(nativeString);
    } else {
        thiz = (jlong)USER_NAME(new)(nativeString);
    }
    (*env)->ReleaseStringUTFChars(env, name, nativeString);
    return thiz;
}

JNI_RETURN(void) JNI_SIGNATURE(delete) (JNIEnv* env, jobject obj, jlong p)
{
    USER_NAME(delete)((USER_THIS) p);
}

JNI_RETURN(jstring) JNI_SIGNATURE(name) (JNIEnv* env, jobject obj, jlong p)
{
    char* n = USER_NAME(name)((USER_THIS) p);
    jstring ret = jniw_to_jstring(env, n);
    USER_NAME(delete_name)(n);
    return ret;
}

JNI_RETURN(jstring) JNI_SIGNATURE(id) (JNIEnv* env, jobject obj, jlong p)
{
    char* n = USER_NAME(id)((USER_THIS) p);
    jstring ret = jniw_to_jstring(env, n);
    USER_NAME(delete_id)(n);
    return ret;
}

JNI_RETURN(jobject) JNI_SIGNATURE(deckSets) (JNIEnv* env, jobject obj, jlong p)
{
    int count;
    int i;

    ygo_deck_DeckSet** ds = USER_NAME(deckSets)((USER_THIS) p, &count);

    jobject arrayList = jniw_arraylist_create_r(env, count);

    /* move the deckset ownership pointer to the java layer */
    jclass deckSetClass = (*env)->FindClass(env,
            "net/sectorsoftware/ygo/deck/DeckSet");
    jmethodID ctor = (*env)->GetMethodID(env, deckSetClass, "<init>", "(J)V");

    for (i = 0; i < count; i++) {
        jobject deckSet = (*env)->NewObject(env, deckSetClass, ctor, ds[i]);
        jniw_arraylist_add(env, arrayList, deckSet);
        (*env)->DeleteLocalRef(env, deckSet);
    }

    USER_NAME(delete_deckSet_array)(ds);

    return arrayList;
}

JNI_RETURN(void) JNI_SIGNATURE(remove) (JNIEnv* env, jobject obj, jlong p)
{
    USER_NAME(remove)((USER_THIS) p);
}

