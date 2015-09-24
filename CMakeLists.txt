cmake_minimum_required(VERSION 2.8)	

project(test3)	

find_package(OpenGL REQUIRED)

set(SOURCE_EXE 
		main.cpp
 		#iniparser/src/iniparser.c
		#iniparser/src/iniparser.h
		)	

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-DINI_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)

include_directories(iniparser/src/
					glm/
					GL/
					glfw3/include/GLFW/
					glew/include/
					#common/iniparser/
					)

### GLFW ###

add_subdirectory (glfw3)
					
### GLEW ###

set(GLEW_SOURCE
	glew/src/glew.c
)

set(GLEW_HEADERS
)


add_library( GLEW_190 STATIC
	${GLEW_SOURCE}
	${GLEW_INCLUDE}
)

target_link_libraries(GLEW_190
	${OPENGL_LIBRARY}
	${EXTRA_LIBS}
)

### INI PARSER ###

set(INI_SOURCE
	iniparser/src/iniparser.c
	iniparser/src/dictionary.c
)

add_library( ini STATIC
	${INI_SOURCE}
	#${GLEW_INCLUDE}
)

target_link_libraries(ini
	#${OPENGL_LIBRARY}
	#${EXTRA_LIBS}
)
					
set(ALL_LIBS
	#${OPENGL_LIBRARY}
	GLFW_303
	GLEW_190
	opengl32
	glu32
	gdi32
	ini
)

add_executable(main ${SOURCE_EXE})	

target_link_libraries(main ${ALL_LIBS} )	



