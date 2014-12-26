cmake_minimum_required(VERSION 2.8)

if ("${CROSS_TARGET}" STREQUAL "linux-64")
    add_subdirectory(qt)
endif()
