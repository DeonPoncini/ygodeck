cmake_minimum_required(VERSION 2.8)
project(ygodeck)

################################
# Compiler setup
################################

set(DECK_VERSION "0.0.1")

################################
# Third party libraries
################################
find_package(kizhi)
find_package(mindbw)
find_package(ygodata)
find_package(zephyr)

include_directories(${KIZHI_INCLUDE_DIRS})
include_directories(${MINDBW_INCLUDE_DIRS})
include_directories(${YGODATA_INCLUDE_DIRS})
include_directories(${ZEPHYR_INCLUDE_DIRS})

################################
# Libraries
################################
set(YGODECK_LIBS
    ${KIZHI_LIBRARIES}
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

add_custom_command(OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/include/ygo/deck/cpp/DataTypes.h
    COMMAND python
    ARGS $ENV{PROJECT_SCRIPT_DIR}/include/generate-data.py
    ${CMAKE_CURRENT_LIST_DIR}/xml/data.xml
    ${CMAKE_CURRENT_BINARY_DIR}/include/ygo/deck)

################################
# Generate database file
################################

make_directory(${CMAKE_CURRENT_BINARY_DIR}/test)
add_custom_command(TARGET ygodeck POST_BUILD
    COMMAND rm ARGS -f ${CMAKE_CURRENT_BINARY_DIR}/test/card.db
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/scripts/load-db
    ARGS ${CMAKE_CURRENT_BINARY_DIR}/test/card.db
)

################################
# Read card data
################################
add_custom_target(ygodeck-card
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/scripts/scrape-cards
    ${CMAKE_CURRENT_LIST_DIR}/card
    ${CMAKE_CURRENT_BINARY_DIR}/error
    ${YGODATA_PYTHON_INCLUDE_DIRS}
    false)

add_custom_target(ygodeck-card-force
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/scripts/scrape-cards
    ${CMAKE_CURRENT_LIST_DIR}/card
    ${CMAKE_CURRENT_BINARY_DIR}/error
    ${YGODATA_PYTHON_INCLUDE_DIRS}
    true)

add_custom_target(ygodeck-format
    COMMAND ${CMAKE_CURRENT_LIST_DIR}/scripts/scrape-formats
    ${CMAKE_CURRENT_LIST_DIR}/format
)

################################
# Export library
################################

export_project(
    NAME ygodeck
    INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_BINARY_DIR}/include
    JAVA_INCLUDES ${CMAKE_CURRENT_BINARY_DIR}/include/ygo/deck/java
    LIBS ygodeck
    RES ${CMAKE_CURRENT_BINARY_DIR}/test/card.db
    PATH ${CMAKE_CURRENT_LIST_DIR}/scripts/
    VERSION ${DECK_VERSION}
)

################################
# Unit tests
################################
add_unit_tests(NAME ygodeck)
