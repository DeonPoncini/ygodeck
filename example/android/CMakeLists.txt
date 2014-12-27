cmake_minimum_required(VERSION 2.8)
project(ygodeck-android)

if(CMAKE_BUILD_TYPE MATCHES "[dD]ebug")
    set(APK_TARGET_NAME "ygodeck-arm-debug-unaligned.apk")
    set(GRADLE_TASK "assembleDebug")
else()
    set(APK_TARGET_NAME "ygodeck-arm-release.apk")
    set(GRADLE_TASK "assembleRelease")
endif()

################################
# Build apk
################################
file(GLOB_RECURSE ANDROID_SRC ./*.java ./*.xml)

set(APK_PATH "${CMAKE_CURRENT_BINARY_DIR}/build/outputs/apk")
set(APK_TARGETS "${APK_PATH}/${APK_TARGET_NAME}")

make_directory(${CMAKE_CURRENT_LIST_DIR}/native-libs/armeabi-v7a)

add_custom_command(
    COMMAND gradlew -p ${CMAKE_CURRENT_LIST_DIR} --project-cache-dir
    ${CMAKE_CURRENT_BINARY_DIR}/.gradle
    -PbuildDir=${CMAKE_CURRENT_BINARY_DIR}/build ${GRADLE_TASK}
    OUTPUT ${APK_TARGETS}
    DEPENDS ${SRC} ${ANDROID_SRC} ${CMAKE_CURRENT_LIST_FILE})

add_custom_target(ygodeckApk ALL DEPENDS ${APK_TARGETS})

