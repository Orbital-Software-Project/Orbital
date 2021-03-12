cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(Orbital)

include_directories(Orbital PRIVATE .)


add_subdirectory(Engine)
add_subdirectory(Editor)

#add_subdirectory(Src)

# Copy shaders
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders DESTINATION ${CMAKE_BINARY_DIR}/Editor)