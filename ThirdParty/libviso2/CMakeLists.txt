# project
cmake_minimum_required (VERSION 2.6)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(libviso2)

# directories
set(LIBVISO2_SRC_DIR src)

# include directory
include_directories("${LIBVISO2_SRC_DIR}")

# use sse3 instruction set
SET(CMAKE_CXX_FLAGS "-msse3")

# sources
set(LIBVISO2_SRC_FILES
    "src/filter.cpp"
    "src/matcher.cpp"
    "src/matrix.cpp"
    "src/reconstruction.cpp"
    "src/triangle.cpp"
    "src/viso.cpp"
    "src/viso_mono.cpp"
    "src/viso_stereo.cpp"
)

# demo program
add_library(libviso2 ${LIBVISO2_SRC_FILES})

target_include_directories(libviso2 PUBLIC "src/")