


$WebClient = New-Object System.Net.WebClient
$WebClient.DownloadFile("https://github.com/microsoft/vcpkg/archive/2020.06.zip",".\2020.06.zip")

Move-Item -Path '.\2020.06.zip' -Destination '..\..\ThirdParty\2020.06.zip'

cd ..\..\ThirdParty\

expand-archive -path '.\2020.06.zip' -destinationpath '.'

Remove-Item 2020.06.zip

cd vcpkg-2020.06

.\bootstrap-vcpkg.bat

.\vcpkg install opencv eigen3 yaml-cpp glog suitesparse glfw3 glew nativefiledialog glm assimp --triplet x64-windows