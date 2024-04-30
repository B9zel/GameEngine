cmake_minimum_required(VERSION 3.2)

set(CORE_PROJECT_NAME CoreEngine)

FILE(GLOB_RECURSE INCLUDES_FILES **.h)
FILE(GLOB_RECURSE SOURCE_FILES **.cpp)


add_library(${CORE_PROJECT_NAME} STATIC 
		${INCLUDES_FILES}
		${SOURCE_FILES}
		)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
		${INCLUDES_FILES}
		${SOURCE_FILES})



target_include_directories(${CORE_PROJECT_NAME} PUBLIC .)
#target_include_directories(${CORE_PROJECT_NAME} PRIVATE .)
target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_17)


add_subdirectory(../libraries/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_PROJECT_NAME} PUBLIC glfw)

add_subdirectory(../libraries/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${CORE_PROJECT_NAME} PUBLIC spdlog)

add_subdirectory(../libraries/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${CORE_PROJECT_NAME} PUBLIC glad)