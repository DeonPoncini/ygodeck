cmake_minimum_required(VERSION 2.8)
project(ygodeck-c)

################################
# Compiler setup
################################

set(C_DECK_VERSION "0.0.1")

################################
# Third party libraries
################################

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

################################
# Build targets
################################
add_library(ygodeck-c
    ${SRC}
    ${INCLUDES})

target_link_libraries(ygodeck-c
    ygodeck)

################################
# Export library
################################

export_project(
    NAME ygodeck-c
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
    LIBS ygodeck-c
    VERSION ${C_DECK_VERSION}
)
