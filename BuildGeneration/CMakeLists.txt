cmake_minimum_required(VERSION 3.20)

set(ProjectName BuildGeneration)

project(${ProjectName})

FILE(GLOB_RECURSE PYTHON_FILES **.py)

add_custom_target(${ProjectName} SOURCES
	${PYTHON_FILES}
)

source_group(${ProjectName} FILES
	${PYTHON_FILES}
)

add_custom_command(
	TARGET ${ProjectName}
	PRE_BUILD
	COMMAND echo Helloooooooooooooooo
	COMMAND python -m venv ${PROJECT_BINARY_DIR}/venv
	COMMAND ${PROJECT_BINARY_DIR}/venv/Scripts/activate
	COMMAND ${PROJECT_BINARY_DIR}/main.py
)

if (FALSE)

find_package(Python3 COMPONENTS Interpreter REQUIRED)



add_custom_command(
	OUTPUT generate_file.txt
	COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Test.py > generate_file.txt
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Test.py
	COMMENT "Running Test.py aaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaaa"
)

message("AFAEGEGEGEGAGAGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGGG")
add_custom_target(generate_data ALL DEPENDS ${CMAKE_BINARY_DIR}/generated_file.txt)

endif()
add_custom_target(setup_python_venv
	COMMAND ${Python3_EXECUTABLE} -m venv ${CMAKE_BINARY_DIR}/venv
	COMMAND ${CMAKE_BINARY_DIR}/venv/bin/pip install -r ${CMAKE_SOURCE_DIR}/tool/requirements.txt
)

install(
	FILES Test1.py
	DESTINATION ${CMAKE_INSTALL_BINDING}/BuildGeneration
)
