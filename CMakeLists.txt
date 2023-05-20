cmake_minimum_required(VERSION 3.5)

set(CMAKE_BUILD_TYPE Debug)
# Project Name
PROJECT(engine)

set(OpenGL_GL_PREFERENCE LEGACY)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)


if (NOT APPLE)
	if (WIN32)
		set(TOOLKITS_FOLDER "" CACHE PATH "Path to toolkits folder")
		include_directories(
			${TOOLKITS_FOLDER}/glew
			${TOOLKITS_FOLDER}/glut
			${TOOLKITS_FOLDER}/devil)
		link_directories(
			${TOOLKITS_FOLDER}/glew
			${TOOLKITS_FOLDER}/glut
			${TOOLKITS_FOLDER}/devil)
	else(WIN32)
		find_package(GLEW REQUIRED)
		include_directories(${GLEW_INCLUDE_DIRS})
		link_libraries(${GLEW_LIBRARIES})
	endif(WIN32)
endif(NOT APPLE)

if (NOT WIN32)
	find_package(GLUT REQUIRED)
	include_directories(${GLUT_INCLUDE_DIR})
	link_directories(${GLUT_LIBRARY_DIRS})
	add_definitions(${GLUT_DEFINITIONS})

	find_package(DevIL REQUIRED)
	link_libraries(${IL_LIBRARIES})
    include_directories(${IL_INCLUDE_DIR})
    add_definitions(${IL_DEFINITIONS})
endif(NOT WIN32)	

find_package(OpenGL REQUIRED)
include_directories(${OpenGL_INCLUDE_DIRS})
link_directories(${OpenGL_LIBRARY_DIRS})
add_definitions(${OpenGL_DEFINITIONS})

add_executable(${PROJECT_NAME} engine/Camera.cpp engine/engine.cpp engine/Group.cpp common/Shape.cpp engine/Parser.cpp common/Point.cpp engine/Transformation.cpp common/MatrixOpp.cpp engine/Curve.cpp)

if (WIN32)
	target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} glut32 glew32 DevIL)
	file(COPY ${TOOLKITS_FOLDER}/glew/glew32.dll DESTINATION ${CMAKE_BINARY_DIR})
	file(COPY ${TOOLKITS_FOLDER}/glut/glut32.dll DESTINATION ${CMAKE_BINARY_DIR})
	file(COPY ${TOOLKITS_FOLDER}/devil/DevIL.dll DESTINATION ${CMAKE_BINARY_DIR})
	set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})

else(WIN32)
    target_link_libraries(${PROJECT_NAME} ${OPENGL_LIBRARIES} ${GLUT_LIBRARIES} ${IL_LIBRARIES})
endif (WIN32)

if(NOT OPENGL_FOUND)
    message(ERROR " OPENGL not found!")
endif(NOT OPENGL_FOUND)
	
PROJECT(generator)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

add_executable(${PROJECT_NAME} common/Shape.cpp gerador/generator.cpp common/Point.cpp gerador/Bezier.cpp common/MatrixOpp.cpp)
