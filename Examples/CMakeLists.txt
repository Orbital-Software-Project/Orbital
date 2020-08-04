cmake_minimum_required(VERSION 2.8.9)

project(ModelViewer)

Set(SRC
	Main.cpp
)

add_executable(ModelViewer ${SRC})
target_link_libraries(ModelViewer PRIVATE Engine glfw GLEW::GLEW)


