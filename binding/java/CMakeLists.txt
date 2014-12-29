cmake_minimum_required(VERSION 2.8)
project(ygodeck-java)

if ("${CROSS_TARGET}" STREQUAL "linux-64")
    find_package(JNI)
    include_directories(${JNI_INCLUDE_DIRS})
elseif ("${CROSS_TARGET}" STREQUAL "android")
    find_package(Android)
    include_directories(${ANDROID_INCLUDE_DIRS})
endif()

################################
# Locate source
################################
file(GLOB_RECURSE SRC jni/*.cpp)
file(GLOB_RECURSE INCLUDES jni/*.h)

add_library(ygodeck-jni SHARED
    ${SRC}
    ${INCLUDES})

if ("${CROSS_TARGET}" STREQUAL "linux-64")
    target_link_libraries(ygodeck-jni
        ${JNI_LIBRARIES})
elseif ("${CROSS_TARGET}" STREQUAL "android")
    target_link_libraries(ygodeck-jni
        ${ANDROID_LIBRARIES})
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
    -PcrossTarget=${CROSS_TARGET} -PbuildDir=${CMAKE_CURRENT_BINARY_DIR}/build
    ${GRADLE_TASK}
    OUTPUT ${JAR_TARGETS}
    DEPENDS ${SRC} ${JAVA_SRC} ${CMAKE_CURRENT_LIST_FILE})

add_custom_target(ygodeckJar ALL DEPENDS ${JAR_TARGETS})
add_dependencies(ygodeckJar ygodeck-jni)
