#include <jni.h>

#include "Utils.h"

#include <jniw/jniw.h>
#include <ygo/deck/c/CardSelector.h>

#define JNI_SIGNATURE(_name) Java_net_sectorsoftware_ygo_deck_CardSelector_##_name

JNI_RETURN(jlong) JNI_SIGNATURE(init) (JNIEnv* env, jobject obj)
{
    return (jlong)(CARD_SEL(new)());
}

JNI_RETURN(void) JNI_SIGNATURE(delete) (JNIEnv* env, jobject obj, jlong p)
{
    CARD_SEL(delete)((CARD_SEL_THIS)p);
}

JNI_RETURN(jobject) JNI_SIGNATURE(query) (JNIEnv* env, jobject obj,
        jlong p, jstring name)
{
    const char* str = (*env)->GetStringUTFChars(env, name, 0);
    ygo_data_StaticCardData* scd = CARD_SEL(query)((CARD_SEL_THIS)p, str);
    (*env)->ReleaseStringUTFChars(env, name, str);

    jobject scdObj = staticDataToJava(env, scd);

    CARD_SEL(query_delete)(scd);

    return scdObj;
}

JNI_RETURN(jobject) JNI_SIGNATURE(execute) (JNIEnv* env, jobject obj, jlong p)
{
    int items;
    int i;
    char** list;
    list = CARD_SEL(execute)((CARD_SEL_THIS)p, &items);

    jclass arrayListClass = (*env)->FindClass(env, "java/util/ArrayList");
    jmethodID ctor = (*env)->GetMethodID(env, arrayListClass, "<init>",
            "(I)V");
    jobject arrayList = (*env)->NewObject(env, arrayListClass, ctor, items);
    jmethodID addMethod = (*env)->GetMethodID(env, arrayListClass, "add",
            "(Ljava/lang/Object;)Z");

    for (i = 0; i < items; i++) {
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

JNI_RETURN(void) JNI_SIGNATURE(cardType)(JNIEnv* env, jobject obj,
        jlong p, jint ct)
{
    CARD_SEL(cardType)((CARD_SEL_THIS)p, (ygo_data_CardType)ct);
}

JNI_RETURN(void) JNI_SIGNATURE(attribute)(JNIEnv* env, jobject obj,
        jlong p, jint a)
{
    CARD_SEL(attribute)((CARD_SEL_THIS)p,(ygo_data_Attribute)a);
}

JNI_RETURN(void) JNI_SIGNATURE(monsterType) (JNIEnv* env, jobject obj,
        jlong p, jint mt)
{
    CARD_SEL(monsterType)((CARD_SEL_THIS)p,(ygo_data_MonsterType)mt);
}

JNI_RETURN(void) JNI_SIGNATURE(type) (JNIEnv* env, jobject obj,
        jlong p, jint t)
{
    CARD_SEL(type)((CARD_SEL_THIS)p,(ygo_data_Type)t);
}

JNI_RETURN(void) JNI_SIGNATURE(level) (JNIEnv* env, jobject obj,
        jlong p, jint l, jint op)
{
    CARD_SEL(level)((CARD_SEL_THIS)p,l,(mindbw_Operator)op);
}

JNI_RETURN(void) JNI_SIGNATURE(attack) (JNIEnv* env, jobject obj,
        jlong p, jint a, jint op)
{
    CARD_SEL(attack)((CARD_SEL_THIS)p,a,(mindbw_Operator)op);
}

JNI_RETURN(void) JNI_SIGNATURE(defense) (JNIEnv* env, jobject obj,
        jlong p, jint d, jint op)
{
    CARD_SEL(defense)((CARD_SEL_THIS)p,d,(mindbw_Operator)op);
}

JNI_RETURN(void) JNI_SIGNATURE(lpendulum) (JNIEnv* env, jobject obj,
        jlong p, jint l, jint op)
{
    CARD_SEL(lpendulum)((CARD_SEL_THIS)p,l,(mindbw_Operator)op);
}

JNI_RETURN(void) JNI_SIGNATURE(rpendulum) (JNIEnv* env, jobject obj,
        jlong p, jint r, jint op)
{
    CARD_SEL(rpendulum)((CARD_SEL_THIS)p,r,(mindbw_Operator)op);
}

JNI_RETURN(void) JNI_SIGNATURE(spellType) (JNIEnv* env, jobject obj,
        jlong p, jint st)
{
    CARD_SEL(spellType)((CARD_SEL_THIS)p,(ygo_data_SpellType)st);
}

JNI_RETURN(void) JNI_SIGNATURE(trapType) (JNIEnv* env, jobject obj,
        jlong p, jint tt)
{
    CARD_SEL(trapType)((CARD_SEL_THIS)p,(ygo_data_TrapType)tt);
}
