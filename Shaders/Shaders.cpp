#include <iostream>

// Future use for spir-v compilation
// Searches for *.vs and *.fs  files etc. and compiles them to *.spv
// ex: Mesh.vs -> Mesh_vs.spv
// ex: Color.fs -> Color_fs.spv

// Creates hashes of the source and compiled files and adds
// them to an entry list for to compile only new or updated shaders
// File: Shader.db

// Fileformat ex:
// #source file name; sha256; compiled file name; sha256;
// MapViewer/Mesh.vs;423422323...;MapViewer/Mesh_vs.spv;24234234...; 

// Commands:
// --clean : removes all *.spv files
// --rebuild : cleans and rebuilds all shaders


int main()
{
    std::cout << "Start Spir-V compilation process\n";



    std::cout << "Compilation process completed\n";

    return 0;
}
