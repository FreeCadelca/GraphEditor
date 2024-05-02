cmake_minimum_required(VERSION 3.17)

project(GraphEditor LANGUAGES CXX)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

add_executable(
        GraphEditor
        main.cpp
        Canvas.h
        Canvas.cpp
        GraphEditorApp.cpp
        GraphEditorApp.h
        Graph.cpp
        Graph.h)

target_compile_features(GraphEditor PRIVATE cxx_std_17)

target_sources(GraphEditor PRIVATE main.cpp)

target_include_directories(GraphEditor PRIVATE ${GTKMM_INCLUDE_DIRS})

target_link_directories(GraphEditor PRIVATE ${GTKMM_LIBRARY_DIRS})

target_link_libraries(GraphEditor PRIVATE ${GTKMM_LIBRARIES})
target_link_libraries(GraphEditor PRIVATE ${GTKMM_LIBRARIES})