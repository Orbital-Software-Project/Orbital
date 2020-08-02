$WebClient = New-Object System.Net.WebClient
$WebClient.DownloadFile("https://github.com/ocornut/imgui/archive/v1.77.zip", ".\v1.77.zip")

Move-Item -Path '.\v1.77.zip' -Destination '..\..\ThirdParty\v1.77.zip'

cd ..\..\ThirdParty\

expand-archive -path '.\v1.77.zip' -destinationpath '.'

Remove-Item v1.77.zip