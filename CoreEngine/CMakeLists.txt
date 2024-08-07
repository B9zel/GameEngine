cmake_minimum_required(VERSION 3.2)

set(CORE_PROJECT_NAME CoreEngine)

FILE(GLOB_RECURSE INCLUDES_FILES **.h)
FILE(GLOB_RECURSE SOURCE_FILES **.cpp)

set(INCLUDE_STB_IMAGE_FILES
	../libraries/Stb_image/stb_image.h)
set(SOURCE_STB_IMAGE_FILES
	../libraries/Stb_image/deprecated/stb_image.c)

add_library(StbImage STATIC
	${INCLUDE_STB_IMAGE_FILES}
	${SOURCE_STB_IMAGE_FILES})
	
	
add_library(${CORE_PROJECT_NAME} STATIC 
		${INCLUDES_FILES}
		${SOURCE_FILES}
		)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
		${INCLUDES_FILES}
		${SOURCE_FILES})


	
target_include_directories(StbImage PUBLIC ../libraries/Stb_image)
target_link_libraries(${CORE_PROJECT_NAME} PUBLIC StbImage)

target_include_directories(${CORE_PROJECT_NAME} PUBLIC .)
target_compile_features(${CORE_PROJECT_NAME} PUBLIC cxx_std_17)


add_subdirectory(../libraries/glfw ${CMAKE_CURRENT_BINARY_DIR}/glfw)
target_link_libraries(${CORE_PROJECT_NAME} PUBLIC glfw)

add_subdirectory(../libraries/spdlog ${CMAKE_CURRENT_BINARY_DIR}/spdlog)
target_link_libraries(${CORE_PROJECT_NAME} PUBLIC spdlog)

add_subdirectory(../libraries/glad ${CMAKE_CURRENT_BINARY_DIR}/glad)
target_link_libraries(${CORE_PROJECT_NAME} PUBLIC glad)

add_subdirectory(../libraries/glm ${SMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${CORE_PROJECT_NAME} PUBLIC glm)
