#!/bin/sh -e

# Make sure the current path is the path the script is located in
cd "$(dirname "$0")"

# Build and install openvslam

git clone https://github.com/Cambloid/openvslam.git

cd openvslam
mkdir build
cd build

export g2o_DIR=../../../ThirdParty/g2o/lib/cmake/g2o
export DBoW2_DIR=../../../ThirdParty/DBoW2/lib/cmake/DBoW2
    
 cmake \
    -DCMAKE_BUILD_TYPE=Release \
    -DCMAKE_TOOLCHAIN_FILE=../../../ThirdParty/vcpkg-2020.06/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_INSTALL_PREFIX=../../../ThirdParty/openvslam/ \
    -DBUILD_WITH_MARCH_NATIVE=ON \
    -DUSE_PANGOLIN_VIEWER=OFF \
    -DUSE_SOCKET_PUBLISHER=OFF \
    -DUSE_STACK_TRACE_LOGGER=ON \
    -DBOW_FRAMEWORK=DBoW2 \
    -DBUILD_TESTS=ON \
    ..
    

make -j4
make install

# Go Back to root dir
cd ../..


# Remove git dir
rm -r -f openvslam
