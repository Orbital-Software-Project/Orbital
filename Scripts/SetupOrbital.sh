#!/bin/sh -e

# Make sure the current path is the path the script is located in
cd "$(dirname "$0")"

# Go one folder back to Orbital root
cd ..


# Build Orbital
mkdir Build
cd Build

export openvslam_DIR=../ThirdParty/openvslam/lib/cmake/openvslam
export g2o_DIR=../ThirdParty/g2o/lib/cmake/g2o
export DBoW2_DIR=../ThirdParty/DBoW2/lib/cmake/DBoW2

cmake                                        \
    -DCMAKE_TOOLCHAIN_FILE=../ThirdParty/vcpkg-2020.06/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_BUILD_TYPE=Debug               \
    -DCMAKE_INSTALL_PREFIX=../Install \
    ..
