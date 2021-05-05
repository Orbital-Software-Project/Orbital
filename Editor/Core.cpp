#include "Editor/Core.h"

#include "Editor/Window.h"




namespace Orb {

Core::Core(std::string rootDir) {
    Global::GetInstance().RootDir = rootDir;
}

Core::~Core() {}

void Core::Run() {
    Window wnd;


    wnd.EnterMsgLoop();
}




}


