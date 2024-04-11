cmake_minimum_required(VERSION 3.17)

project(untitled1 LANGUAGES CXX)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

add_executable(untitled1
        Canvas.cpp
        Canvas.h
        bfs.cpp)

target_compile_features(untitled1 PRIVATE cxx_std_17)

target_sources(untitled1 PRIVATE main.cpp)

target_include_directories(untitled1 PRIVATE ${GTKMM_INCLUDE_DIRS})

target_link_directories(untitled1 PRIVATE ${GTKMM_LIBRARY_DIRS})

target_link_libraries(untitled1 PRIVATE ${GTKMM_LIBRARIES})