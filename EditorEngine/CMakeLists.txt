cmake_minimum_required(VERSION 3.20)

set(EDITOR_PROJECT_NAME EditorEngine)

FILE(GLOB_RECURSE INCLUDES_FILES **.h)
FILE(GLOB_RECURSE SOURCE_FILES **.cpp)


add_executable(${EDITOR_PROJECT_NAME}
				${INCLUDES_FILES}
				${SOURCE_FILES})
				
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${INCLUDES_FILES} ${SOURCE_FILES})


set(INCLUDE_IMGUI_FILES
	../libraries/imgui/imgui.h
	../libraries/imgui/backends/imgui_impl_glfw.h
	../libraries/imgui/backends/imgui_impl_opengl3.h
)
set(SOURCE_IMGUI_FILES 
	../libraries/imgui/imgui.cpp
	../libraries/imgui/backends/imgui_impl_glfw.cpp
	../libraries/imgui/backends/imgui_impl_opengl3.cpp
	../libraries/imgui/imgui_draw.cpp
	../libraries/imgui/imgui_tables.cpp
	../libraries/imgui/imgui_widgets.cpp
)


add_library(ImGui STATIC
			${INCLUDE_IMGUI_FILES}
			${SOURCE_IMGUI_FILES})

target_include_directories(ImGui PUBLIC ../libraries)
target_include_directories(ImGui PUBLIC ../libraries/imgui)

target_link_libraries(${EDITOR_PROJECT_NAME} CoreEngine ImGui)

target_link_libraries(ImGui PRIVATE glad glfw)

target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin/)
