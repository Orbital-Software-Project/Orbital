
git clone https://github.com/shinsumicco/DBoW2.git

cd DBoW2
New-Item -Path "." -Name "build" -ItemType "directory"
cd build

cmake `
    -DCMAKE_TOOLCHAIN_FILE=../../../../ThirdParty/vcpkg-2020.06/scripts/buildsystems/vcpkg.cmake `
    -DCMAKE_BUILD_TYPE=Debug `
    -DCMAKE_INSTALL_PREFIX=../../../../ThirdParty/DBoW2/ `
    ..



# Go Back to root dir
#cd ../..

# Remove git dir
#Remove-Item DBoW2

CMD /c PAUSE