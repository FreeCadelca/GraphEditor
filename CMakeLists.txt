cmake_minimum_required(VERSION 3.27)
project(GraphEditor)

set(CMAKE_CXX_STANDARD 14)

# Установка пути к локально распакованному GoogleTest
set(GOOGLETEST_ROOT "${CMAKE_SOURCE_DIR}/external/googletest-1.14.0")

add_subdirectory(${GOOGLETEST_ROOT} googletest)

# Подавление предупреждений о C++11 расширениях
add_compile_options(-Wno-c++11-extensions)

add_executable(
        GraphEditor
        main.cpp
        Canvas.h
        Canvas.cpp
        GraphEditorApp.cpp
        GraphEditorApp.h
        Graph.cpp
        Graph.h
        Color.cpp
        Color.h
        VertexEntryDialog.cpp
        VertexEntryDialog.h
        WeightEntryDialog.cpp
        WeightEntryDialog.h
)

target_compile_features(GraphEditor PRIVATE cxx_std_17)

target_sources(GraphEditor PRIVATE main.cpp)

target_include_directories(GraphEditor PRIVATE ${GTKMM_INCLUDE_DIRS})

target_link_directories(GraphEditor PRIVATE ${GTKMM_LIBRARY_DIRS})

target_link_libraries(GraphEditor PRIVATE ${GTKMM_LIBRARIES})


# Добавление тестового исполняемого файла
add_executable(GraphTests
        Graph_tests.cpp
        Graph.cpp
        Graph.h
        Canvas.cpp
        Color.cpp
)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

set(SOURCE_FILES main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES})


target_include_directories(GraphTests PRIVATE ${GTKMM_INCLUDE_DIRS} ${GOOGLETEST_ROOT}/googletest/include)
target_link_directories(GraphTests PRIVATE ${GTKMM_LIBRARY_DIRS})
target_link_libraries(GraphTests PRIVATE gtest gtest_main ${GTKMM_LIBRARIES})

# Включение тестирования и добавление тестов
enable_testing()
add_test(NAME GraphTests COMMAND GraphTests)