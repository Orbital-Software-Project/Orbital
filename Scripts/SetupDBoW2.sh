#!/bin/sh -e

# Make sure the current path is the path the script is located in
cd "$(dirname "$0")"


# Build and install DBoW2

git clone https://github.com/shinsumicco/DBoW2.git

cd DBoW2
mkdir build && cd build

cmake                                        \
    -DCMAKE_TOOLCHAIN_FILE=../../../ThirdParty/vcpkg-2020.06/scripts/buildsystems/vcpkg.cmake \
    -DCMAKE_BUILD_TYPE=Release               \
    -DCMAKE_INSTALL_PREFIX=../../../ThirdParty/DBoW2/ \
    ..
    

make -j4
make install

# Go Back to root dir
cd ../..


# Remove git dir
rm -r -f DBoW2
