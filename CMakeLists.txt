cmake_minimum_required(VERSION 2.8)
project(ygodeck)

################################
# Compiler setup
################################

set(DECK_VERSION "0.0.1")

################################
# Third party libraries
################################
find_package(mindbw)
find_package(ygodata)
find_package(zephyr)

include_directories(${MINDBW_INCLUDE_DIRS})
include_directories(${YGODATA_INCLUDE_DIRS})
include_directories(${ZEPHYR_INCLUDE_DIRS})

################################
# Libraries
################################
set(YGODECK_LIBS
    ${MINDBW_LIBRARIES}
    ${YGODATA_LIBRARIES}
    ${ZEPHYR_LIBRARIES}
    )

################################
# Locate source
################################
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SRC src/*.cpp)

include_directories(include)
include_directories(${CMAKE_CURRENT_BINARY_DIR}/include)

################################
# Build targets
################################
add_library(ygodeck
    ${SRC}
    ${INCLUDES}
    ${CMAKE_CURRENT_BINARY_DIR}/include/ygo/deck/cpp/DataTypes.h)

target_link_libraries(ygodeck
    ${YGODECK_LIBS})

make_directory(${CMAKE_CURRENT_BINARY_DIR}/include/ygo/deck/cpp)
make_directory(${CMAKE_CURRENT_BINARY_DIR}/include/ygo/deck/c)
make_directory(${CMAKE_CURRENT_BINARY_DIR}/include/ygo/deck/java)

add_custom_command(OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/include/ygo/deck/cpp/DataTypes.h
    COMMAND python
    ARGS $ENV{PROJECT_SCRIPT_DIR}/include/generate-data.py
    ${CMAKE_CURRENT_LIST_DIR}/xml/data.xml
    ${CMAKE_CURRENT_BINARY_DIR}/include/ygo/deck)

################################
# Export library
################################

export_project(
    NAME ygodeck
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include
    LIBS ygodeck
    VERSION ${DECK_VERSION}
)

add_subdirectory(binding)
add_subdirectory(example)
