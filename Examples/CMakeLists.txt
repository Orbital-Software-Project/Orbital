cmake_minimum_required(VERSION 2.8.9)

project(Examples)

Set(SRC
	Main.cpp
)

add_executable(ModelViewer ${SRC})
target_link_libraries(ModelViewer PRIVATE Engine glfw GLEW::GLEW)

file(COPY ${CMAKE_SOURCE_DIR}/Shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR})




