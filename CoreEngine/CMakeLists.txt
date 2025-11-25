cmake_minimum_required(VERSION 3.30)

set(CORE_PROJECT_NAME CoreEngine)
set(OutputPythonBuildPathName "OutputGenFiles")
set(OutputPythonBuildPath "")

    set(CMAKE_CXX_STANDARD 17)
    set(CMAKE_CXX_STANDARD_REQUIRED ON)

FILE(GLOB_RECURSE INCLUDES_FILES **.h)
FILE(GLOB_RECURSE SOURCE_FILES **.cpp)

set(STRING_RESULT "")

string(ASCII 27 Esc)
file( READ "${CMAKE_SOURCE_DIR}/BuildGeneration/BuildGenConfig.ini" LIB_PATH_STR)
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
                    set(OutputPythonBuildPath ${VALUE})
                    break()
                endif()
            endif()
    endif()
endforeach()

message(STATUS ${CMAKE_CURRENT_SOURCE_DIR}/${OutputPythonBuildPath})
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${OutputPythonBuildPath})
    message(STATUS HERHEH)
    FILE(GLOB_RECURSE GENERATED_FILES ${CMAKE_CURRENT_SOURCE_DIR}/${OutputPythonBuildPath}/**.h)
    FILE(GLOB_RECURSE GENERATED_FILES_CPP ${CMAKE_CURRENT_SOURCE_DIR}/${OutputPythonBuildPath}/**.cpp)
endif()
include_directories(${OutputPythonBuildPath})

# Create stb_image library
set(INCLUDE_STB_IMAGE_FILES
	../libraries/Stb_image/stb_image.h)
set(SOURCE_STB_IMAGE_FILES
	../libraries/Stb_image/deprecated/stb_image.c)

add_library(StbImage STATIC
	${INCLUDE_STB_IMAGE_FILES}
	${SOURCE_STB_IMAGE_FILES})
#

#FILE(GLOB_RECURSE INCLUDE_JSON ../libraries/json/include/**.hpp)
#FILE(GLOB_RECURSE SRC_JSON ../libraries/json/**.cpp)
#add_library(Json STATIC 
#    ${INCLUDE_JSON} 
#    ${SRC_JSON}
#)
add_library(Json INTERFACE)


source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES
		${INCLUDES_FILES}
		${SOURCE_FILES}
		)
target_include_directories(Json INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}/../libraries/json/include
)

	
	
add_library(${CORE_PROJECT_NAME} STATIC 
		${INCLUDES_FILES}
		${SOURCE_FILES}
		${GENERATED_FILES}
		${GENERATED_FILES_CPP}
		)


target_include_directories(${CORE_PROJECT_NAME} PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${OutputPythonBuildPath}>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/)
set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/${OutputPythonBuildPath} PROPERTIES GENERATED TRUE)



	
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

add_subdirectory(../libraries/glm ${CMAKE_CURRENT_BINARY_DIR}/glm)
target_link_libraries(${CORE_PROJECT_NAME} PUBLIC glm)

add_subdirectory(../libraries/assimp ${CMAKE_CURRENT_BINARY_DIR}/assimp)
target_link_libraries(${CORE_PROJECT_NAME} PUBLIC assimp)


#add_subdirectory(../libraries/json ${CMAKE_CURRENT_BINARY_DIR}/json)
#target_include_directories(Json PUBLIC ../libraries/json/include)
target_link_libraries(${CORE_PROJECT_NAME} PUBLIC Json)
#target_compile_features(Json PUBLIC cxx_std_17)


