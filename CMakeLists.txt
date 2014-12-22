cmake_minimum_required(VERSION 2.8)
project(libygodeck)

################################
# Compiler setup
################################

set(DECK_VERSION "0.0.1")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DBOOST_LOG_DYN_LINK")

################################
# Third party libraries
################################
find_package(Boost 1.55.0 REQUIRED)
find_package(mindbw)
find_package(libygodata)

include_directories(${BOOST_INCLUDE_DIRS})
include_directories(${SQLITE3_INCLUDE_DIR})
include_directories(${MINDBW_INCLUDE_DIRS})
include_directories(${LIBYGODATA_INCLUDE_DIRS})

################################
# Libraries
################################
set(YGODECK_LIBS
    ${Boost_LIBRARIES}
    ${MINDBW_LIBRARIES}
    ${LIBYGODATA_LIBRARIES}
    )

################################
# Locate source
################################
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SRC src/*.cpp)

include_directories(include)

################################
# Build targets
################################
add_library(ygodeck SHARED
    ${SRC}
    ${INCLUDES})

target_link_libraries(ygodeck
    ${YGODECK_LIBS})

################################
# Export library
################################

export_project(
    NAME libygodeck
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
    LIBS ygodeck
    VERSION ${DECK_VERSION}
)

