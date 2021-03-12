@echo off

mkdir Build
cd Build

cmake -S .. --preset=msvc-x64-static-debug
cmake -S .. --preset=msvc-x64-static-release

pause






