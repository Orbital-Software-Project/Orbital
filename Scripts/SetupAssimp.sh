#!/bin/sh -e


# Make sure the current path is the path the script is located in
cd "$(dirname "$0")"

wget https://github.com/assimp/assimp/archive/v5.0.1.zip

unzip v5.0.1.zip
rm -f v5.0.1.zip

cd assimp-5.0.1

mkdir -p build
cd build

 cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=../../../ThirdParty/assimp/ \
    -DASSIMP_BUILD_TESTS=OFF \
    ..

make -j4
make install

cd ../..
rm -r -f assimp-5.0.1

