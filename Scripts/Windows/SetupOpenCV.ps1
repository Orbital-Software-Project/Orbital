
$WebClient = New-Object System.Net.WebClient
$WebClient.DownloadFile("https://github.com/opencv/opencv/archive/3.4.0.zip",".\3.4.0.zip")

expand-archive -path '.\3.4.0.zip' -destinationpath '.'

cd opencv-3.4.0
New-Item -Path "." -Name "build" -ItemType "directory"
cd build


cmake `
    -G "Visual Studio 16 2019" -A x64 `
    -DCMAKE_BUILD_TYPE=Debug `
    -DENABLE_CXX11=ON `
    -DBUILD_DOCS=OFF `
    -DBUILD_EXAMPLES=OFF `
    -DBUILD_JASPER=OFF `
    -DBUILD_OPENEXR=OFF `
    -DBUILD_PERF_TESTS=OFF `
    -DBUILD_TESTS=OFF `
    -DWITH_EIGEN=ON `
    -DWITH_FFMPEG=ON `
    -DWITH_OPENMP=ON `
    ..

make -j4
make install

# Go Back to root dir
cd ../..

# Remove git dir
Remove-Item opencv-3.4.0