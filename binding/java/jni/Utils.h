#include <jni.h>

#include <ygo/deck/c/CardSelector.h>

jobject staticDataToJava(JNIEnv* env, ygo_data_StaticCardData* scd);
