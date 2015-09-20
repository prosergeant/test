cmake_minimum_required(VERSION 2.8)	 # Проверка версии CMake.
                                        # Если версия установленой программы
                                        # старее указаной, произайдёт аварийный выход.

project(test3)			# Название проекта

set(SOURCE_EXE main.cpp)		# Установка переменной со списком исходников для исполняемого файла

#set(HEADER commands.hpp)

#set(SOURCE_LIB hello.cpp)			# Тоже самое, но для библиотеки

#add_library(hello STATIC ${SOURCE_LIB})	# Создание статической библиотеки с именем foo

include_directories(inisrc glm GL common)

add_executable(main ${SOURCE_EXE})	# Создает исполняемый файл с именем main

add_subdirectory(glm)
add_subdirectory(inisrc) #для сборки надо указать такойже путь или мб common/iniparser/src
add_subdirectory(glfw3)

#glfw3 GL GLU GLEW ini stdc++ X11 Xxf86vm Xrandr pthread Xi Xinerama Xcursor

target_link_libraries(main glfw GL GLU GLEW ini stdc++ X11 Xxf86vm Xrandr pthread Xi Xinerama Xcursor)		# Линковка программы с библиотекой
