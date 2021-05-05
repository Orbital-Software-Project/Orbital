#include <iostream>
#include "Core.h"



int main(int argc, char *argv[]) {
	
	std::string rootDir = std::string(argv[0]);
	rootDir = rootDir.substr(0, rootDir.find_last_of("/\\"));

    Orb::Core core(rootDir);
    core.Run();
	
	
	return 0;
}
