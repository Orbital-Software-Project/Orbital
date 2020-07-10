#!/bin/sh -e

# Make sure the current path is the path the script is located in
cd "$(dirname "$0")"


# Build and install g2o

wget http://bitbucket.org/eigen/eigen/get/3.3.4.tar.bz2
tar xf 3.3.4.tar.bz2
rm -rf 3.3.4.tar.bz2
cd eigen-eigen-5a0156e40feb

mkdir -p build && cd build

cmake \
    -DCMAKE_BUILD_TYPE=Debug \
    -DCMAKE_INSTALL_PREFIX=../../../ThirdParty/Eigen3/  \
    ..
    
make -j4
make install


# Go Back to script dir
cd ../..


# Remove eigen dir
#rm -r -f eigen-eigen-5a0156e40feb

