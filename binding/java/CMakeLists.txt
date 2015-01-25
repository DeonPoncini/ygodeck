cmake_minimum_required(VERSION 2.8)
project(ygodeck-java)

set(DECK_JAVA_VERSION "0.0.1")

if ("${CROSS_TARGET}" STREQUAL "linux-64")
    find_package(JNI)
    include_directories(${JNI_INCLUDE_DIRS})
elseif ("${CROSS_TARGET}" STREQUAL "android")
    find_package(Android)
    include_directories(${ANDROID_INCLUDE_DIRS})
endif()

find_package(jniw)
find_package(ygodata)
find_package(ygodata-jni)
find_package(mindbw)
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
# Generate database file
################################

make_directory(${CMAKE_CURRENT_BINARY_DIR}/test)
add_custom_command(TARGET ygodeck-jni POST_BUILD
    COMMAND rm ARGS -f ${CMAKE_CURRENT_BINARY_DIR}/test/card.db
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/../../scripts/load-db
    ARGS ${CMAKE_CURRENT_BINARY_DIR}/test/card.db
)

################################
# Build jar
################################
file(GLOB_RECURSE JAVA_SRC ./*.java)

set(JAR_TARGET_NAME "ygodeck.jar")

set(JAR_PATH "${CMAKE_CURRENT_BINARY_DIR}/build/libs")
set(JAR_TARGETS "${JAR_PATH}/${JAR_TARGET_NAME}")

if ("${CROSS_TARGET}" STREQUAL "linux-64")
    get_filename_component(JAVA_AWT_DIR ${JAVA_AWT_LIBRARY} PATH)
endif()

add_custom_command(
    COMMAND gradlew
    -p ${CMAKE_CURRENT_LIST_DIR}
    --project-cache-dir ${CMAKE_CURRENT_BINARY_DIR}/gradle
    -PnativeDir=${CMAKE_CURRENT_BINARY_DIR}/native-libs/
    -PygodataIncludes=${YGODATA_JAVA_INCLUDE_DIRS}
    -PygodeckIncludes=${YGODECK-JAVA-DATA_JAVA_INCLUDE_DIRS}
    -PmindbwIncludes=${MINDBW_JAVA_INCLUDE_DIRS}
    -ParchiveNativeDir=${ARCHIVE_NATIVE_DIR}
    -PjavaAwtDir=${JAVA_AWT_DIR}
    -PdbDir=${CMAKE_CURRENT_BINARY_DIR}/test/card.db
    -PbuildDir=${CMAKE_CURRENT_BINARY_DIR}/build
    build -x test
    OUTPUT ${JAR_TARGETS}
    DEPENDS ${SRC} ${JAVA_SRC} ${CMAKE_CURRENT_LIST_FILE})

add_custom_target(ygodeckJar ALL DEPENDS ${JAR_TARGETS})
add_dependencies(ygodeckJar ygodeck-jni)

add_custom_target(ygodeckJar-test
    COMMAND gradlew
    -p ${CMAKE_CURRENT_LIST_DIR}
    --project-cache-dir ${CMAKE_CURRENT_BINARY_DIR}/gradle
    -PnativeDir=${CMAKE_CURRENT_BINARY_DIR}/native-libs/
    -PygodataIncludes=${YGODATA_JAVA_INCLUDE_DIRS}
    -PygodeckIncludes=${YGODECK-JAVA-DATA_JAVA_INCLUDE_DIRS}
    -PmindbwIncludes=${MINDBW_JAVA_INCLUDE_DIRS}
    -ParchiveNativeDir=${ARCHIVE_NATIVE_DIR}
    -PjavaAwtDir=${JAVA_AWT_DIR}
    -PdbDir=${CMAKE_CURRENT_BINARY_DIR}/test/card.db
    -PbuildDir=${CMAKE_CURRENT_BINARY_DIR}/build
    test)

add_dependencies(ygodeckJar-test ygodeckJar)
add_dependencies(check ygodeckJar-test)

################################
# Export jar
################################
export_project(
    NAME ygodeck-java
    JARS ${JAR_TARGETS}
    JAR_PATHS ${JAR_PATH}
    VERSION ${DECK_JAVA_VERSION}
)
