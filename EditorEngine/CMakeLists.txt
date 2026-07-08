cmake_minimum_required(VERSION 3.1...3.50)

set(EDITOR_PROJECT_NAME EditorEngine)
set(OutputPythonBuildPathName "OutputGenFiles")
set(OutputPythonBuildPath "")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB_RECURSE INCLUDES_FILES **.h)
FILE(GLOB_RECURSE SOURCE_FILES **.cpp)

set(STRING_RESULT "")

string(ASCII 27 Esc)
file( READ "${CMAKE_SOURCE_DIR}/BuildGeneration/BuildGenConfig.ini" LIB_PATH_STR)


message(fefefefefefefe)
string(REGEX REPLACE "\n" "${Esc};" LIB_PATH_LINES "${LIB_PATH_STR}")
foreach(LINE ${LIB_PATH_LINES})
    string(FIND ${LINE} "=" POSITION)
    if(NOT "${POSITION}" STREQUAL "-1")
            #remove the key, leave the content untouch
            string(REPLACE "${KEY}" "" STRING_RESULT ${LINE})
            # Swap the appended Esc character back out in favour of a line feed
            string(REGEX REPLACE "${Esc}" "" STRING_RESULT ${STRING_RESULT})
            string(STRIP ${STRING_RESULT} STRING_RESULT)
            string(FIND ${STRING_RESULT} "=" POSITION)
            if (NOT ${POSITION} STREQUAL "-1" )
                string(SUBSTRING ${STRING_RESULT} 0 ${POSITION} KEY)
                string(STRIP ${KEY} KEY)
                math(EXPR POSITION "${POSITION} + 1")
                string(SUBSTRING ${STRING_RESULT} ${POSITION} -1 VALUE)
                string(STRIP ${VALUE} VALUE)
                if (${KEY} STREQUAL ${OutputPythonBuildPathName})
                message(STATUS ${KEY} " ========= "  ${VALUE})
                    set(OutputPythonBuildPath ${VALUE}/${EDITOR_PROJECT_NAME})
                    break()
                endif()
            endif()
    endif()
endforeach()

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/${OutputPythonBuildPath})
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${OutputPythonBuildPath})
    message(STATUS HERHEH)
    message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/${OutputPythonBuildPath})
    FILE(GLOB_RECURSE GENERATED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${OutputPythonBuildPath}/**.h)
    FILE(GLOB_RECURSE GENERATED_FILES_CPP ${CMAKE_CURRENT_SOURCE_DIR}/${OutputPythonBuildPath}/**.cpp)
endif()
include_directories(${OutputPythonBuildPath})






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
				${SOURCE_FILES}
                ${GENERATED_FILES}
                ${GENERATED_FILES_CPP}
)


target_include_directories(ImGui PUBLIC ../libraries)
target_include_directories(ImGui PUBLIC ../libraries/imgui)
target_include_directories(ImGuizmo PUBLIC ../libraries/imgui)
target_include_directories(${EDITOR_PROJECT_NAME} PUBLIC .)

target_link_libraries(${EDITOR_PROJECT_NAME} PUBLIC CoreEngine ImGui ImGuizmo)
target_link_libraries(ImGui PRIVATE glad glfw)


target_compile_features(${EDITOR_PROJECT_NAME} PUBLIC cxx_std_17)

set_target_properties(${EDITOR_PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/Bin/)
