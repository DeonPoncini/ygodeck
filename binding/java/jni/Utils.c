#include "Utils.h"

#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

jstring c_string_to_java(JNIEnv* env, char* str)
{
    int len = strlen(str);
    jbyte* nativeStr = (jbyte*)(str);
    jbyteArray bytes = (*env)->NewByteArray(env, (jsize)(len));
    (*env)->SetByteArrayRegion(env, bytes, 0, len, nativeStr);

    jclass charsetClass = (*env)->FindClass(env, "java/nio/charset/Charset");
    jmethodID forName = (*env)->GetStaticMethodID(env, charsetClass,
            "forName", "(Ljava/lang/String;)Ljava/nio/charset/Charset;");
    jstring utf8 = (*env)->NewStringUTF(env, "UTF-8");
    jobject charset = (*env)->CallStaticObjectMethod(env, charsetClass,
            forName, utf8);

    jclass stringClass = (*env)->FindClass(env, "java/lang/String");
    jmethodID ctor = (*env)->GetMethodID(env, stringClass, "<init>",
            "([BLjava/nio/charset/Charset;)V");

    jstring ret = (jstring)((*env)->NewObject(env, stringClass,
                ctor, bytes, charset));

    return ret;
}

#ifdef __cplusplus
}
#endif
