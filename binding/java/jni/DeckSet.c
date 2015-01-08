#include <jni.h>

#include <jniw/jniw.h>
#include <ygo/data/jni/DataUtils.h>
#include <ygo/deck/c/DeckSet.h>

#define JNI_SIGNATURE(_name) Java_net_sectorsoftware_ygo_deck_DeckSet_##_name

JNI_RETURN(jlong) JNI_SIGNATURE(init) (JNIEnv* env, jobject obj, jstring name,
        jlong user, jlong format)
{
    const char* str = (*env)->GetStringUTFChars(env, name, 0);
    jlong thiz = (jlong) DECKSET_NAME(new)(str, (USER_THIS) user,
            (FORMAT_THIS) format);
    (*env)->ReleaseStringUTFChars(env, name, str);
    return thiz;
}

JNI_RETURN(jlong) JNI_SIGNATURE(initCreate) (JNIEnv* env, jobject obj,
        jstring name, jlong user, jlong format)
{
    const char* str = (*env)->GetStringUTFChars(env, name, 0);
    jlong thiz = (jlong) DECKSET_NAME(create)(str, (USER_THIS) user,
            (FORMAT_THIS) format);
    (*env)->ReleaseStringUTFChars(env, name, str);
    return thiz;
}

JNI_RETURN(void) JNI_SIGNATURE(delete) (JNIEnv* env, jobject obj, jlong p)
{
    DECKSET_NAME(delete)((DECKSET_THIS) p);
}

JNI_RETURN(jstring) JNI_SIGNATURE(name) (JNIEnv* env, jobject obj, jlong p)
{
    char* n = DECKSET_NAME(name)((DECKSET_THIS) p);
    jstring ret = jniw_to_jstring(env, n);
    DECKSET_NAME(delete_name)(n);
    return ret;
}

JNI_RETURN(jlong) JNI_SIGNATURE(format) (JNIEnv* env, jobject obj, jlong p)
{
    return (jlong) DECKSET_NAME(format)((DECKSET_THIS) p);
}

JNI_RETURN(jint) JNI_SIGNATURE(addCard) (JNIEnv* env, jobject obj, jlong p,
        jint deckType, jstring name)
{
    const char* str = (*env)->GetStringUTFChars(env, name, 0);
    jint ret = (jint) DECKSET_NAME(addCard)((DECKSET_THIS) p,
            (ygo_data_DeckType) deckType, str);
    (*env)->ReleaseStringUTFChars(env, name, str);
    return ret;
}

JNI_RETURN(jobject) JNI_SIGNATURE(cards) (JNIEnv* env, jobject obj, jlong p)
{
    ygo_deck_CardMap ydc = DECKSET_NAME(cards)((DECKSET_THIS) p);

    /* hash map */
    jclass hashMapClass = (*env)->FindClass(env, "java/util/HashMap");
    jmethodID ctor = (*env)->GetMethodID(env, hashMapClass, "<init>", "()V");
    jmethodID mapPutMethod = (*env)->GetMethodID(env, hashMapClass, "put",
            "(Ljava/lang/Object;Ljava/lang/Object;)Ljava/lang/Object;");
    jobject hashMap = (*env)->NewObject(env, hashMapClass, ctor);

    /* main deck list */
    jclass arrayListClass = (*env)->FindClass(env, "java/util/ArrayList");
    jmethodID arrayAddMethod = (*env)->GetMethodID(env, arrayListClass, "add",
            "(Ljava/lang/Object;)Z");

    /* deck type enum values */
    jclass deckTypeClass = (*env)->FindClass(env,
            "net/sectorsoftware/ygo/data/DataTypes$DeckType");
    jfieldID deckTypeMainField = (*env)->GetStaticFieldID(env, deckTypeClass,
            "MAIN", "L/net/sectorsoftware/ygo/data/DataTypes$DeckType;");
    jfieldID deckTypeSideField = (*env)->GetStaticFieldID(env, deckTypeClass,
            "SIDE", "L/net/sectorsoftware/ygo/data/DataTypes$DeckType;");
    jfieldID deckTypeExtraField = (*env)->GetStaticFieldID(env, deckTypeClass,
            "EXTRA", "L/net/sectorsoftware/ygo/data/DataTypes$DeckType;");

    jobject deckTypeMain = (*env)->GetStaticObjectField(env, deckTypeClass,
            deckTypeMainField);
    jobject deckTypeSide = (*env)->GetStaticObjectField(env, deckTypeClass,
            deckTypeSideField);
    jobject deckTypeExtra = (*env)->GetStaticObjectField(env, deckTypeClass,
            deckTypeExtraField);

    jobject mainList = jniw_arraylist_create_r(env, ydc.main_count);
    jobject sideList = jniw_arraylist_create_r(env, ydc.side_count);
    jobject extraList = jniw_arraylist_create_r(env, ydc.extra_count);

    for (int i = 0; i < ydc.main_count; i++) {
        ygo_data_StaticCardData* scd = ydc.main[i];
        jobject scdObj = ygo_data_static_card_data_to_java(env, scd);
        (*env)->CallBooleanMethod(env, mainList, arrayAddMethod, scdObj);
        (*env)->DeleteLocalRef(env, scdObj);
    }
    (*env)->CallObjectMethod(env, hashMap, mapPutMethod,
            deckTypeMain, mainList);

    for (int i = 0; i < ydc.side_count; i++) {
        ygo_data_StaticCardData* scd = ydc.side[i];
        jobject scdObj = ygo_data_static_card_data_to_java(env, scd);
        (*env)->CallBooleanMethod(env, sideList, arrayAddMethod, scdObj);
        (*env)->DeleteLocalRef(env, scdObj);
    }
    (*env)->CallObjectMethod(env, hashMap, mapPutMethod,
            deckTypeSide, sideList);

    for (int i = 0; i < ydc.extra_count; i++) {
        ygo_data_StaticCardData* scd = ydc.extra[i];
        jobject scdObj = ygo_data_static_card_data_to_java(env, scd);
        (*env)->CallBooleanMethod(env, extraList, arrayAddMethod, scdObj);
        (*env)->DeleteLocalRef(env, scdObj);
    }
    (*env)->CallObjectMethod(env, hashMap, mapPutMethod,
            deckTypeExtra, extraList);

    DECKSET_NAME(delete_cards)(ydc);

    return hashMap;
}

JNI_RETURN(void) JNI_SIGNATURE(deleteCard) (JNIEnv* env, jobject obj, jlong p,
        jint deckType, jstring name)
{
    const char* str = (*env)->GetStringUTFChars(env, name, 0);
    DECKSET_NAME(deleteCard)((DECKSET_THIS) p,
            (ygo_data_DeckType) deckType, str);
    (*env)->ReleaseStringUTFChars(env, name, str);
}

JNI_RETURN(void) JNI_SIGNATURE(remove) (JNIEnv* env, jobject obj, jlong p)
{
    DECKSET_NAME(remove)((DECKSET_THIS) p);
}

JNI_RETURN(jboolean) JNI_SIGNATURE(validate) (JNIEnv* env, jobject obj, jlong p)
{
    return (jboolean) DECKSET_NAME(validate)((DECKSET_THIS) p);
}
