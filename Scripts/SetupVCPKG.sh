#!/bin/sh -e


# Make sure the current path is the path the script is located in
cd "$(dirname "$0")"

wget https://github.com/microsoft/vcpkg/archive/2020.06.zip

mv 2020.06.zip ../ThirdParty/

cd ../ThirdParty/
unzip 2020.06.zip

rm -f 2020.06.zip

cd vcpkg-2020.06

bash ./bootstrap-vcpkg.sh

./vcpkg install opencv yaml-cpp eigen3 glog suitesparse glfw3
