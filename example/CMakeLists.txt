cmake_minimum_required(VERSION 2.8)

project(ygodeck-example)

if ("${CROSS_TARGET}" STREQUAL "linux-64")
    add_subdirectory(qt)
elseif ("${CROSS_TARGET}" STREQUAL "android")
    add_subdirectory(android)
endif()
