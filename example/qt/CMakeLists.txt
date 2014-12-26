cmake_minimum_required(VERSION 2.8)
project(ygodeck-qt)

################################
# Compiler setup
################################

set(YGO_QT_VERSION "0.0.1")
set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

find_package(Qt5Widgets REQUIRED)

################################
# Locate source
################################
file(GLOB_RECURSE SRC src/*.cpp)

################################
# Build targets
################################
add_executable(ygodeck-qt
    ${SRC})
target_link_libraries(ygodeck-qt
    ygodeck)

################################
# Export library
################################
export_project(
    NAME ygodeck-qt
    BINS ygodeck-qt
    VERSION ${YGO_QT_VERSION}
)

qt5_use_modules(ygodeck-qt Widgets)
