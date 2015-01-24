cmake_minimum_required(VERSION 2.8)
project(ygodeck-c)

################################
# Compiler setup
################################

set(C_DECK_VERSION "0.0.1")

################################
# Third party libraries
################################
find_package(ygodata-c)

################################
# Locate source
################################
file(GLOB_RECURSE INCLUDES include/*.h)
file(GLOB_RECURSE SRC src/*.cpp)

include_directories(include)
include_directories(${YGODATA-C_INCLUDE_DIRS})

################################
# Build targets
################################
add_library(ygodeck-c
    ${SRC}
    ${INCLUDES})

target_link_libraries(ygodeck-c
    ygodeck
    ${YGODATA-C_LIBRARIES})

################################
# Generate database file
################################

make_directory(${CMAKE_CURRENT_BINARY_DIR}/test)
add_custom_command(TARGET ygodeck-c POST_BUILD
    COMMAND rm ARGS -f ${CMAKE_CURRENT_BINARY_DIR}/test/card.db
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/../../scripts/load-db
    ARGS ${CMAKE_CURRENT_BINARY_DIR}/test/card.db
)

################################
# Export library
################################

export_project(
    NAME ygodeck-c
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
    LIBS ygodeck-c
    VERSION ${C_DECK_VERSION}
)

################################
# Unit tests
################################
add_unit_tests(NAME ygodeck-c)
