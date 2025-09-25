cmake_minimum_required(VERSION 3.20)

set(PROJECT_NAME GameEngine)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Bin/Output_Direction)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Bin/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/Bin/lib)
set(CMAKE_BINARY_DIR ${CMAKE_BINARY_DIR}/Bin/Dir)

project(${PROJECT_NAME})


add_subdirectory(CoreEngine)
add_subdirectory(EditorEngine)
add_subdirectory(BuildGeneration)



set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT EditorEngine)

