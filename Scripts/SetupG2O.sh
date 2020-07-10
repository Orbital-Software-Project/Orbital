#!/bin/sh -e

# Make sure the current path is the path the script is located in
cd "$(dirname "$0")"


# Build and install g2o

git clone https://github.com/RainerKuemmerle/g2o.git

cd g2o
git checkout 9b41a4ea5ade8e1250b9c1b279f3a9c098811b5a
mkdir build && cd build

 cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_TOOLCHAIN_FILE=../../../ThirdParty/vcpkg-2020.06/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_INSTALL_PREFIX=../../../ThirdParty/g2o/ \
    -DCMAKE_CXX_FLAGS=-std=c++11 \
    -DBUILD_SHARED_LIBS=ON \
    -DBUILD_UNITTESTS=OFF \
    -DBUILD_WITH_MARCH_NATIVE=OFF \
    -DG2O_USE_CHOLMOD=OFF \
    -DG2O_USE_CSPARSE=ON \
    -DG2O_USE_OPENGL=OFF \
    -DG2O_USE_OPENMP=ON \
    ..
    

make -j4
make install

# Go Back to root dir
cd ../..


# Remove git dir
rm -r -f g2o
