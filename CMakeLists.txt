cmake_minimum_required(VERSION 2.8.9)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# path to dependencies
set(CMAKE_TOOLCHAIN_FILE ./ThirdParty/vcpkg-2020.06/scripts/buildsystems/vcpkg.cmake)
set(OpenCV_DIR ./ThirdParty/opencv/share/OpenCV)
set(assimp_DIR ./ThirdParty/assimp/lib/cmake/assimp-5.0)

project(Orbital)

# Required for openvslam
find_package(suitesparse CONFIG REQUIRED)

# Window and OpenGL
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)

find_package(OpenCV REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

#Add OpenVSLAM as source
add_subdirectory(ThirdParty)


add_subdirectory(Engine)
add_subdirectory(Examples)
add_subdirectory(Editor)

# Copy shaders
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Editor)
