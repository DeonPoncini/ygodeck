cmake_minimum_required(VERSION 2.8)
project(ygodeck-android)

find_package(ygodeck-java)

if(CMAKE_BUILD_TYPE MATCHES "[dD]ebug")
    set(APK_TARGET_NAME "android-debug.apk")
    set(GRADLE_TASK "assembleDebug")
else()
    set(APK_TARGET_NAME "android-release.apk")
    set(GRADLE_TASK "assembleRelease")
endif()

################################
# Build apk
################################
file(GLOB_RECURSE ANDROID_SRC ./*.java ./*.xml)

set(APK_PATH "${CMAKE_CURRENT_BINARY_DIR}/build/outputs/apk")
set(APK_TARGETS "${APK_PATH}/${APK_TARGET_NAME}")

add_custom_command(
    COMMAND gradlew -p ${CMAKE_CURRENT_LIST_DIR} --project-cache-dir
    ${CMAKE_CURRENT_BINARY_DIR}/gradle
    -PlibraryPath=${YGODECK-JAVA_JAR_PATHS}
    -PbuildDir=${CMAKE_CURRENT_BINARY_DIR}/build ${GRADLE_TASK}
    OUTPUT ${APK_TARGETS}
    DEPENDS ${ANDROID_SRC} ${CMAKE_CURRENT_LIST_FILE} ygodeckJar)

add_custom_target(ygodeckApk ALL DEPENDS ${APK_TARGETS})

################################
# Export apk
################################
export_java_project(
    NAME ygodeck-android
    APKS ${APK_TARGETS}
)
