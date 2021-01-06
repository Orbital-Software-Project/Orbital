cmake_minimum_required(VERSION 3.5)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_TOOLCHAIN_FILE ./ThirdParty/vcpkg-2020.06/scripts/buildsystems/vcpkg.cmake)


project(Orbital)


# Required for openvslam
find_package(suitesparse CONFIG REQUIRED)

# Window and OpenGL
find_package(GLEW REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenCV REQUIRED)
find_package(unofficial-nativefiledialog CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)


include_directories(.)

add_subdirectory(ThirdParty)
add_subdirectory(ThirdParty/libmv)
add_subdirectory(ThirdParty/libviso2)

add_subdirectory(Engine)
add_subdirectory(Editor)


# Copy shaders
file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/Shaders DESTINATION ${CMAKE_CURRENT_BINARY_DIR}/Editor)

