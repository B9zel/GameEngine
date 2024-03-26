cmake_minimum_required(VERSION 3.2)

set(PROJECT_NAME GameEngine)

project(${PROJECT_NAME})

add_subdirectory(CoreEngine)
add_subdirectory(EditorEngine)


set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT EditorEngine)

