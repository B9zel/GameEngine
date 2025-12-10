cmake_minimum_required(VERSION 3.1...3.50)

set(EDITOR_PROJECT_NAME EditorEngine)

FILE(GLOB_RECURSE INCLUDES_FILES **.h)
FILE(GLOB_RECURSE SOURCE_FILES **.cpp)



				
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
	../libraries/imgui/imgui_demo.cpp
)

set(INCLUDE_IMGUIZMO_FILES
	../libraries/ImGuizmo/ImGuizmo.h
	../libraries/ImGuizmo/example/imgui.h
)
set(SOURCE_IMGUIZMO_FILES
	../libraries/ImGuizmo/ImGuizmo.cpp
	../libraries/ImGuizmo/example/imgui.cpp
)

add_library(ImGui STATIC
			${INCLUDE_IMGUI_FILES}
			${SOURCE_IMGUI_FILES}
)

add_library(ImGuizmo STATIC
	${INCLUDE_IMGUIZMO_FILES}
	${SOURCE_IMGUIZMO_FILES}
)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
		${INCLUDES_FILES}
		${SOURCE_FILES})

add_executable(${EDITOR_PROJECT_NAME}
				${INCLUDES_FILES}
				${SOURCE_FILES})


target_include_directories(ImGui PUBLIC ../libraries)
target_include_directories(ImGui PUBLIC ../libraries/imgui)
target_include_directories(ImGuizmo PUBLIC ../libraries/imgui)
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC .)

target_link_libraries(${EDITOR_PROJECT_NAME} PUBLIC CoreEngine ImGui ImGuizmo)
target_link_libraries(ImGui PRIVATE glad glfw)


target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin/)
