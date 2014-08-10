cmake_minimum_required(VERSION 2.8)
project(libygodeck)

################################
# Compiler setup
################################

add_definitions(-Wall)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -g -DBOOST_LOG_DYN_LINK")
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
    "${CMAKE_SOURCE_DIR}/../cmake/modules/")

################################
# Third party libraries
################################
find_package(Boost 1.55.0 REQUIRED)
find_package(SQLite3 REQUIRED)

include_directories(${BOOST_INCLUDE_DIRS})
include_directories(${SQLITE3_INCLUDE_DIR})
include_directories(${CMAKE_SOURCE_DIR}/../libygodata/include/)

################################
# Libraries
################################
set(YGODECK_LIBS
    ${Boost_LIBRARIES}
    ${SQLITE3_LIBRARIES}
    ygodata
    )

################################
# Locate source
################################
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SRC src/*.cpp)

include_directories(include)

################################
# Resources
################################

################################
# Libraries
################################

################################
# Build targets
################################
add_library(ygodeck SHARED
    ${SRC}
    ${INCLUDES})

target_link_libraries(ygodeck
    ${YGODECK_LIBS})
