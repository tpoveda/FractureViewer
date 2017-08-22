#########################################
######## Fracture Games - CMake #########
#########################################

cmake_minimum_required(VERSION 2.6)

project(${PROJECTNAME})

#fijamos los flags de -c (compilacion) y -Wall
add_definitions(
	-c
	-W1
)

if ( CMAKE_BUILD_TYPE STREQUAL "" )
	# CMake defaults to leaving CMAKE_BUILD_TYPE empty. This screws up
	# differentiation between debug and release builds.
	set(
		CMAKE_BUILD_TYPE
			"Debug"
		CACHE STRING
			"Choose the type of build, options are: None (CMAKE_CXX_FLAGS or CMAKE_C_FLAGS used) \"Debug\" \"Release\" \"RelWithDebInfo\" \"MinSizeRel\"."
		FORCE
	)
endif ( CMAKE_BUILD_TYPE STREQUAL "" )

if ( CMAKE_BUILD_TYPE STREQUAL "Release" )
add_definitions( -O2 )
endif ( CMAKE_BUILD_TYPE STREQUAL "Release" )

#Cargamos nuestras cabeceras
file(GLOB_RECURSE CABECERAS ${${PROJECTNAME}_SOURCE_DIR}/src/*.h)

#Cargamos todo nuestro codigo fuente
file(GLOB_RECURSE FUENTES ${${PROJECTNAME}_SOURCE_DIR}/src/*.cpp)

#Definimos el ejecutable
add_executable(${PROJECTNAME} ${CABECERAS} ${FUENTES})

#Definimos un CMAKE_DIR para encontrar dependencias externas (SDL2)
set(${PROJECTNAME}_CMAKE_DIR "${${PROJECTNAME}_SOURCE_DIR}/cmake")

#############################################
### Buscamos los paquetes que necesitamos ###
#############################################

#OpenGL
find_package(OpenGL REQUIRED)

# GLEW
INCLUDE(${${PROJECTNAME}_CMAKE_DIR}/FindGLEW.cmake)

#SDL2
INCLUDE(${${PROJECTNAME}_CMAKE_DIR}/FindSDL2.cmake)

#GLM
INCLUDE(${${PROJECTNAME}_CMAKE_DIR}/FindGLM.cmake)

#ASSIMP
INCLUDE(${${PROJECTNAME}_CMAKE_DIR}/FindASSIMP.cmake)

#DevIL
INCLUDE(${${PROJECTNAME}_CMAKE_DIR}/FindDevIL.cmake)

#IRRLICHT
INCLUDE(${${PROJECTNAME}_CMAKE_DIR}/FindIrrlicht.cmake)

#BULLET
INCLUDE(${${PROJECTNAME}_CMAKE_DIR}/FindBullet.cmake)

#BEGUI
INCLUDE(${${PROJECTNAME}_CMAKE_DIR}/FindBeGUI.cmake)

#FREETYPE
INCLUDE(${${PROJECTNAME}_CMAKE_DIR}/FindFreetype.cmake)

#RAKNET
INCLUDE(${${PROJECTNAME}_CMAKE_DIR}/FindRakNet.cmake)

#Definimos los directorios de inclusion
include_directories(
	${FractureViewer_SOURCE_DIR}/headers
	${FractureViewer_SOURCE_DIR}/sources
	${OPENGL_INCLUDE_DIRS}
	${GLEW_INCLUDE_DIRS}
	${SDL2_INCLUDE_DIRS}
	${GLM_INCLUDE_DIRS}
	${ASSIMP_INCLUDE_DIRS}
	${DevIL_INCLUDE_DIRS}
	${IRRLICHT_INCLUDE_DIRS}
	${BULLET_INCLUDE_DIR1}
	${BULLET_INCLUDE_DIR11}
	${BULLET_INCLUDE_DIR12}
	${BULLET_INCLUDE_DIR13}
	${BULLET_INCLUDE_DIR14}
	${BULLET_INCLUDE_DIR15}
	${BULLET_INCLUDE_DIR2}
	${BULLET_INCLUDE_DIR21}
	${BULLET_INCLUDE_DIR22}
	${BULLET_INCLUDE_DIR23}
	${BULLET_INCLUDE_DIR24}
	${BULLET_INCLUDE_DIR25}
	${BULLET_INCLUDE_DIR26}
	${BULLET_INCLUDE_DIR3}
	${BEGUI_INCLUDE_DIRS}
	${FREETYPE_INCLUDE_DIR1}
	${FREETYPE_INCLUDE_DIR2}
	${RAKNET_INCLUDE_DIR}
)

#Definimos las dependencias de librerias adicionales
target_link_libraries( FractureViewer
	${OPENGL_LIBRARIES}
	${GLEW_LIBRARIES}
	${SDL2_LIBRARIES}
	${SDL2_LIBRARIES_MAIN}
	${ASSIMP_LIBRARIES}
	${DevIL_LIBRARIES}
	${DevIL_LIBRARIES2}
	${DevIL_LIBRARIES3}
	${IRRLICHT_LIBRARIES}
	${BULLET_LIBRARY1}
	${BULLET_LIBRARY2}
	${BULLET_LIBRARY3}
	${BEGUI_LIBRARY1}
	${BEGUI_LIBRARY2}
	${FREETYPE_LIBRARIES}
	${RAKNET_LIBRARIES}
)


