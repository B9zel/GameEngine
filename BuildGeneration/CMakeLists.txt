cmake_minimum_required(VERSION 3.30)

set(ProjectName BuildGeneration)
set(PYTHON_VENV ${CMAKE_CURRENT_SOURCE_DIR}/venv/Scripts/python.exe)

project(${ProjectName})

FILE(GLOB_RECURSE PYTHON_FILES **.py)



source_group(${ProjectName} FILES
	${PYTHON_FILES}
)

add_custom_target(${ProjectName} ALL
      COMMAND ${CMAKE_COMMAND} -E echo "Running ${PYTHON_VENV} ${CMAKE_CURRENT_SOURCE_DIR}/ParsFile.py"
    COMMAND "${PYTHON_VENV}" "${CMAKE_CURRENT_SOURCE_DIR}/ParsFile.py"
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    SOURCES ${PYTHON_FILES}
)





if (FALSE)

add_custom_target(${ProjectName} ALL
	${PYTHON_FILES}
	
)

add_custom_command(
	TARGET ${ProjectName}
	PRE_BUILD
	COMMAND echo ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/BuildGeneration/GeneralFile.py
	COMMAND echo fegag
	COMMAND ${CMAKE_BINARY_DIR}/venv/Scripts/activate
	COMMAND ${Python_EXECUTABLE} ${CMAKE_BINARY_DIR}/BuildGeneration/GeneralFile.py
)


find_package(Python3 COMPONENTS Interpreter REQUIRED)



add_custom_command(
	OUTPUT generate_file.txt
	COMMAND ${Python3_EXECUTABLE} ${CMAKE_SOURCE_DIR}/Test.py > generate_file.txt
	WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
	DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Test.py
)


add_custom_target(generate_data ALL DEPENDS ${CMAKE_BINARY_DIR}/generated_file.txt)

add_custom_target(setup_python_venv
	COMMAND ${Python3_EXECUTABLE} -m venv ${CMAKE_BINARY_DIR}/venv
	COMMAND ${CMAKE_BINARY_DIR}/venv/bin/pip install -r ${CMAKE_SOURCE_DIR}/tool/requirements.txt
)

install(
	FILES Test1.py
	DESTINATION ${CMAKE_INSTALL_BINDING}/BuildGeneration
)
endif()
