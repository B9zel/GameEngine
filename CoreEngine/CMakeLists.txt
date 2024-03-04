cmake_minimum_required(VERSION 3.2)

set(CORE_PROJECT_NAME CoreEngine)

FILE(GLOB PUBLIC_INCLUDES_FILES Public/includes/CoreEngine/Core/*.h)
FILE(GLOB PUBLIC_SOURCE_FILES Public/source/CoreEngine/Core/*.cpp)

FILE(GLOB PRIVATE_INCLUDES_FILES Private/includes/CoreEngine/Core/*.h)
FILE(GLOB PRIVATE_SOURCE_FILES Private/source/CoreEngine/Core/*.cpp)

add_library(${CORE_PROJECT_NAME} STATIC
		${PUBLIC_INCLUDES_FILES}
		${PUBLIC_SOURCE_FILES}
		${PRIVATE_INCLUDES_FILES}
		${PRIVATE_SOURCE_FILES})

target_include_directories(${CORE_PROJECT_NAME} PUBLIC Public/includes)
target_include_directories(${CORE_PROJECT_NAME} PRIVATE .)

target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_17)

add_subdirectory(../libraries/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_PROJECT_NAME} PRIVATE glfw)