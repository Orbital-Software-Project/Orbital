cmake_minimum_required(VERSION 3.6)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS OFF)
set(VCPKG_TARGET_TRIPLET x64-windows-static)

project(Orbital)


find_package(GLEW REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)


add_subdirectory(ThirdParty)

include_directories(Orbital PRIVATE .)

#add_subdirectory(Engine)
add_subdirectory(Editor)


# Copy shaders
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders DESTINATION ${CMAKE_BINARY_DIR}/Editor)