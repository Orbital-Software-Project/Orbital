cmake_minimum_required(VERSION 2.8.9)


project(Orbital)

find_package(openvslam REQUIRED)


Set(SRC
	Editor/Main.cpp
)

add_executable(Orbital ${SRC})
