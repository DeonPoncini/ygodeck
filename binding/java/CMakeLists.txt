cmake_minimum_required(VERSION 2.8)
project(ygodeck-java)

if ("${CROSS_TARGET}" STREQUAL "linux-64")
    find_package(JNI)
    include_directories(${JNI_INCLUDE_DIRS})
elseif ("${CROSS_TARGET}" STREQUAL "android")
    find_package(Android)
    include_directories(${ANDROID_INCLUDE_DIRS})
endif()

find_package(jniw)
find_package(ygodata-java)
find_package(ygodata-jni)
find_package(mindbw-java)
find_package(ygodeck-java-data)

include_directories(${JNIW_INCLUDE_DIRS})
include_directories(${YGODATA-JNI_INCLUDE_DIRS})

################################
# Locate source
################################
file(GLOB_RECURSE SRC jni/*.c)
file(GLOB_RECURSE INCLUDES jni/*.h)

add_library(ygodeck-jni SHARED
    ${SRC}
    ${INCLUDES})

set(YGODECK_JNI_3P_LIBS
    ygodeck-c
    ${JNIW_LIBRARIES}
    ${YGODATA-JNI_LIBRARIES})

if ("${CROSS_TARGET}" STREQUAL "linux-64")
    target_link_libraries(ygodeck-jni
        ${JNI_LIBRARIES}
        ${YGODECK_JNI_3P_LIBS})
    set(ARCHIVE_NATIVE_DIR "linux-64")
elseif ("${CROSS_TARGET}" STREQUAL "android")
    target_link_libraries(ygodeck-jni
        ${ANDROID_LIBRARIES}
        ${YGODECK_JNI_3P_LIBS})
    set(ARCHIVE_NATIVE_DIR "armeabi")
endif()

set_target_properties(ygodeck-jni PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/native-libs/)

################################
# Build jar
################################
file(GLOB_RECURSE JAVA_SRC ./*.java)

set(JAR_TARGET_NAME "ygodeck.jar")
set(GRADLE_TASK "build")

set(JAR_PATH "${CMAKE_CURRENT_BINARY_DIR}/build/libs")
set(JAR_TARGETS "${JAR_PATH}/${JAR_TARGET_NAME}")

add_custom_command(
    COMMAND gradlew -p ${CMAKE_CURRENT_LIST_DIR} --project-cache-dir
    ${CMAKE_CURRENT_BINARY_DIR}/gradle
    -PnativeDir=${CMAKE_CURRENT_BINARY_DIR}/native-libs/
    -PygodataIncludes=${YGODATA-JAVA_INCLUDES}
    -PygodeckIncludes=${YGODECK-JAVA-DATA_INCLUDES}
    -PmindbwIncludes=${MINDBW-JAVA_INCLUDES}
    -ParchiveNativeDir=${ARCHIVE_NATIVE_DIR}
    -PbuildDir=${CMAKE_CURRENT_BINARY_DIR}/build
    ${GRADLE_TASK}
    OUTPUT ${JAR_TARGETS}
    DEPENDS ${SRC} ${JAVA_SRC} ${CMAKE_CURRENT_LIST_FILE})

add_custom_target(ygodeckJar ALL DEPENDS ${JAR_TARGETS})
add_dependencies(ygodeckJar ygodeck-jni)

################################
# Export jar
################################
export_java_project(
    NAME ygodeck-java
    JARS ${JAR_TARGETS}
    JAR_PATHS ${JAR_PATH}
)
