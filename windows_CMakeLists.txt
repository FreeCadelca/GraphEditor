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
        src/main.cpp
        src/Canvas/Canvas.h
        src/Canvas/Canvas.cpp
        src/GraphEditorApp/GraphEditorApp.cpp
        src/GraphEditorApp/GraphEditorApp.h
        src/Graph/Graph.cpp
        src/Graph/Graph.h
        src/Color/Color.cpp
        src/Color/Color.h
        src/Dialogs/VertexEntryDialog/VertexEntryDialog.cpp
        src/Dialogs/VertexEntryDialog/VertexEntryDialog.h
        src/Dialogs/WeightEntryDialog/WeightEntryDialog.cpp
        src/Dialogs/WeightEntryDialog/WeightEntryDialog.h
        src/Dialogs/PathEntryDialog/PathEntryDialog.cpp
        src/Dialogs/PathEntryDialog/PathEntryDialog.h
)

find_package(PkgConfig)
pkg_check_modules(GTKMM gtkmm-3.0)

include_directories(${GTKMM_INCLUDE_DIRS})
link_directories(${GTKMM_LIBRARY_DIRS})

set(SOURCE_FILES src/main.cpp)
target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES})

target_compile_features(GraphEditor PRIVATE cxx_std_17)

target_sources(GraphEditor PRIVATE src/main.cpp)

target_include_directories(GraphEditor PRIVATE ${GTKMM_INCLUDE_DIRS})

target_link_directories(GraphEditor PRIVATE ${GTKMM_LIBRARY_DIRS})

#target_link_libraries(GraphEditor PRIVATE ${GTKMM_LIBRARIES})

# Добавление тестового исполняемого файла
add_executable(
        GraphTests
        tests/Graph_tests.cpp
        src/Graph/Graph.cpp
        src/Graph/Graph.h
        src/Canvas/Canvas.cpp
        src/Color/Color.cpp
)

target_include_directories(GraphTests PRIVATE ${GTKMM_INCLUDE_DIRS} ${GOOGLETEST_ROOT}/googletest/include)
target_link_directories(GraphTests PRIVATE ${GTKMM_LIBRARY_DIRS})
target_link_libraries(GraphTests PRIVATE gtest gtest_main ${GTKMM_LIBRARIES})

add_executable(
        GraphEditorAppTests
        tests/GraphEditorApp_tests.cpp
        src/Canvas/Canvas.h
        src/Canvas/Canvas.cpp
        src/GraphEditorApp/GraphEditorApp.cpp
        src/GraphEditorApp/GraphEditorApp.h
        src/Graph/Graph.cpp
        src/Graph/Graph.h
        src/Color/Color.cpp
        src/Color/Color.h
        src/Dialogs/VertexEntryDialog/VertexEntryDialog.cpp
        src/Dialogs/VertexEntryDialog/VertexEntryDialog.h
        src/Dialogs/WeightEntryDialog/WeightEntryDialog.cpp
        src/Dialogs/WeightEntryDialog/WeightEntryDialog.h
        src/Dialogs/PathEntryDialog/PathEntryDialog.cpp
        src/Dialogs/PathEntryDialog/PathEntryDialog.h
)

target_include_directories(GraphEditorAppTests PRIVATE ${GTKMM_INCLUDE_DIRS} ${GOOGLETEST_ROOT}/googletest/include)
target_link_directories(GraphEditorAppTests PRIVATE ${GTKMM_LIBRARY_DIRS})
target_link_libraries(GraphEditorAppTests PRIVATE gtest gmock gtest_main ${GTKMM_LIBRARIES})

# Включение тестирования и добавление тестов
enable_testing()
add_test(NAME GraphTests COMMAND GraphTests)
add_test(NAME GraphEditorAppTests COMMAND GraphEditorAppTests)