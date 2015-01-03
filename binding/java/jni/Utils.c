#include "Utils.h"

#include <jniw/jniw.h>

jobject staticDataToJava(JNIEnv* env, ygo_data_StaticCardData* scd)
{
    jclass scdClass = (*env)->FindClass(env,
            "net/sectorsoftware/ygo/data/DataTypes$StaticCardData");
    jmethodID scdCtor = (*env)->GetMethodID(env, scdClass,
            "<init>", "()V");

    jobject scdObj = (*env)->NewObject(env, scdClass, scdCtor);

    set_string_field(env, scdClass, scdObj, "name", scd->name);

    jfieldID j = (*env)->GetFieldID(env, scdClass,
            "cardType",
            "Lnet/sectorsoftware/ygo/data/DataTypes$CardType;");

    set_enum_field(env, scdClass, scdObj, "cardType",
            "Lnet/sectorsoftware/ygo/data/DataTypes$CardType;",
            (jsize)scd->cardType);

    set_enum_field(env, scdClass, scdObj, "attribute",
            "Lnet/sectorsoftware/ygo/data/DataTypes$Attribute;",
            (jsize)scd->attribute);

    set_enum_field(env, scdClass, scdObj, "monsterType",
            "Lnet/sectorsoftware/ygo/data/DataTypes$MonsterType;",
            (jsize)scd->monsterType);

    set_enum_field(env, scdClass, scdObj, "type",
            "Lnet/sectorsoftware/ygo/data/DataTypes$Type;",
            (jsize)scd->type);

    set_enum_field(env, scdClass, scdObj, "monsterAbility",
            "Lnet/sectorsoftware/ygo/data/DataTypes$MonsterType;",
            (jsize)scd->monsterAbility);

    set_int_field(env, scdClass, scdObj, "level", scd->level);
    set_int_field(env, scdClass, scdObj, "attack", scd->attack);
    set_int_field(env, scdClass, scdObj, "defense", scd->defense);
    set_int_field(env, scdClass, scdObj, "lpendulum", scd->lpendulum);
    set_int_field(env, scdClass, scdObj, "rpendulum", scd->rpendulum);

    set_enum_field(env, scdClass, scdObj, "spellType",
            "Lnet/sectorsoftware/ygo/data/DataTypes$SpellType;",
            (jsize)scd->spellType);

    set_enum_field(env, scdClass, scdObj, "trapType",
            "Lnet/sectorsoftware/ygo/data/DataTypes$TrapType;",
            (jsize)scd->trapType);

    set_string_field(env, scdClass, scdObj, "text", scd->text);

    return scdObj;
}
