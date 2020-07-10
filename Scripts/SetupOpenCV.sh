#!/bin/sh -e

# Make sure the current path is the path the script is located in
cd "$(dirname "$0")"


# Build and install g2o

wget https://github.com/opencv/opencv/archive/3.4.0.zip

unzip 3.4.0.zip
rm -rf 3.4.0.zip
cd opencv-3.4.0

mkdir -p build && cd build


cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=../../../ThirdParty/opencv/ \
    -DENABLE_CXX11=ON \
    -DBUILD_DOCS=OFF \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_JASPER=OFF \
    -DBUILD_OPENEXR=OFF \
    -DBUILD_PERF_TESTS=OFF \
    -DBUILD_TESTS=OFF \
    -DWITH_EIGEN=ON \
    -DWITH_FFMPEG=ON \
    -DWITH_OPENMP=ON \
    ..
    

make -j4
make install

# Go Back to root dir
cd ../..


# Remove git dir
rm -r -f opencv-3.4.0
