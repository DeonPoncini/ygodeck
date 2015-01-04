#include <jni.h>

#include <jniw/jniw.h>
#include <ygo/data/jni/DataUtils.h>
#include <ygo/deck/c/Deck.h>

#define JNI_SIGNATURE(_name) Java_net_sectorsoftware_ygo_deck_Deck_##_name

JNI_RETURN(jlong) JNI_SIGNATURE(init) (JNIEnv* env, jobject obj, jint deckType)
{
    return (jlong) DECK_NAME(new) ((ygo_data_DeckType) deckType);
}

JNI_RETURN(jlong) JNI_SIGNATURE(init_id) (JNIEnv* env, jobject obj,
        jint deckType, jstring id)
{
    const char* str = (*env)->GetStringUTFChars(env, id, 0);
    jlong ptr = (jlong) DECK_NAME(new_id) ((ygo_data_DeckType) deckType, str);
    (*env)->ReleaseStringUTFChars(env, id, str);
    return ptr;
}

JNI_RETURN(void) JNI_SIGNATURE(delete) (JNIEnv* env, jobject obj, jlong p)
{
    DECK_NAME(delete) ((DECK_THIS) p);
}

JNI_RETURN(jint) JNI_SIGNATURE(deckType) (JNIEnv* env, jobject obj, jlong p)
{
    return (jint) DECK_NAME(deckType)((DECK_THIS) p);
}

JNI_RETURN(jstring) JNI_SIGNATURE(id) (JNIEnv* env, jobject obj, jlong p)
{
    char* i = DECK_NAME(id)((DECK_THIS) p);
    jstring s = jniw_to_jstring(env, i);
    DECK_NAME(delete_id)(i);
    return s;
}

JNI_RETURN(jint) JNI_SIGNATURE(size) (JNIEnv* env, jobject obj, jlong p)
{
    return (jint) DECK_NAME(size)((DECK_THIS) p);
}

JNI_RETURN(jint) JNI_SIGNATURE(addCard) (JNIEnv* env, jobject obj,
        jlong p, jstring name)
{
    const char* str = (*env)->GetStringUTFChars(env, name, 0);
    jint err = (jint) DECK_NAME(addCard)((DECK_THIS) p, str);
    (*env)->ReleaseStringUTFChars(env, name, str);
    return err;
}

JNI_RETURN(jobject) JNI_SIGNATURE(cards) (JNIEnv* env, jobject obj, jlong p)
{
    int i;
    int count;

    ygo_data_StaticCardData** scdList = DECK_NAME(cards)((DECK_THIS) p, &count);

    jobject arrayList = jniw_arraylist_create_r(env, count);

    for (i = 0; i < count; i++) {
        ygo_data_StaticCardData* scd = scdList[count];
        jobject scdObj = ygo_data_static_card_data_to_java(env, scd);
        jniw_arraylist_add(env, arrayList, scdObj);
    }

    DECK_NAME(delete_cards)(scdList, count);

    return arrayList;
}

JNI_RETURN(void) JNI_SIGNATURE(deleteCard) (JNIEnv* env, jobject obj, jlong p,
        jstring name)
{
    const char* str = (*env)->GetStringUTFChars(env, name, 0);
    DECK_NAME(deleteCard)((DECK_THIS) p, str);
    (*env)->ReleaseStringUTFChars(env, name, str);
}

