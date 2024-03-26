cmake_minimum_required(VERSION 3.2)

set(EDITOR_PROJECT_NAME EditorEngine)

FILE(GLOB INCLUDES_FILES includes/EditorEngine/*.h)
FILE(GLOB SOURCE_FILES source/EditorEngine/*.cpp)


add_executable(${EDITOR_PROJECT_NAME}
				${INCLUDES_FILES}
				${SOURCE_FILES})
				
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCLUDES_FILES} ${SOURCE_FILES})

target_link_libraries(${EDITOR_PROJECT_NAME} CoreEngine)
target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin/)
