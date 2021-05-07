#include <iostream>

#include "Core.h"

/*
 Architecture:
	- Immediade mode philosophy
	- Hierarchy of classes with certain task
		- Core
			- Window
				- IView (Taskpanel, Sequencer, MapViewer, Outliner)
	- Classes are specialised to certain tasks
		- Ex. Window class creates Glfw windows and handles their events

 VS Project Des:
	- Editor
		- UI handling
		- Behaviour of Orbital
	- Engine
		- Scenegraph classes
		- Loading 3d models
		- Opengl render to texture
		- (not yet) handle all opengl/gpu calls
	- Shaders
		- glsl shaders
		- Future: Small app to compile glsl shaders to spirv

 Class Desc:
	- Window : create and manage native GltfWindow and handle Glfw messages

 General Behaviors:

 Behavior - Startup and Runtime:
	- Main creates core
	- Core class has instance of Window
	- Window class holds instances of IViews
	- IViews provide ViewRequests which get evaluated by Window/Core class

 Behavior - Create OpenVSLAMTask
	- IView Taskpanel creates instance of ITask OpenVSLAMTask
	- IView Taskpanel adds OpenVSLAMTask to TaskWorker collection
	- IView Taskpanel displays all created ITask and displays progress

*/

int main(int argc, char *argv[]) {
	
	// Get dir of the executable
	std::string rootDir = std::string(argv[0]);
	rootDir = rootDir.substr(0, rootDir.find_last_of("/\\"));


    Orb::Core core(rootDir);
    core.Run();
	
	
	return 0;
}
