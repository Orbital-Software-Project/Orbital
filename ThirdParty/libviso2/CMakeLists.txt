# project
cmake_minimum_required (VERSION 2.6)

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

# make release version
#set(CMAKE_BUILD_TYPE Release)

# demo program
add_library(viso2 ${LIBVISO2_SRC_FILES})

# target_link_libraries (viso2 png)
