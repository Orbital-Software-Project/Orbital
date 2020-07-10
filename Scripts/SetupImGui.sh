#!/bin/sh -e


# Make sure the current path is the path the script is located in
cd "$(dirname "$0")"

wget https://github.com/ocornut/imgui/archive/v1.77.zip

mv v1.77.zip ../ThirdParty/

cd ../ThirdParty/
unzip v1.77.zip

rm -f v1.77.zip
