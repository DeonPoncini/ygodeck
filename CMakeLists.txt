cmake_minimum_required(VERSION 2.8)
project(ygodeck)

################################
# Compiler setup
################################

set(DECK_VERSION "0.0.1")

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

################################
# Third party libraries
################################
find_package(Qt5Widgets REQUIRED)

find_package(mindbw)
find_package(ygodata)
find_package(zephyr)

include_directories(${MINDBW_INCLUDE_DIRS})
include_directories(${SQLITE3_INCLUDE_DIR})
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
file(GLOB_RECURSE TEST test/*.cpp)

include_directories(include)

################################
# Build targets
################################
add_library(ygodeck SHARED
    ${SRC}
    ${INCLUDES})

target_link_libraries(ygodeck
    ${YGODECK_LIBS})

add_executable(ygodeck-qt ${TEST})

target_link_libraries(ygodeck-qt
    ygodeck)

################################
# Export library
################################

export_project(
    NAME ygodeck
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
    BINS ygodeck-qt
    LIBS ygodeck
    VERSION ${DECK_VERSION}
)

qt5_use_modules(ygodeck-qt Widgets)
