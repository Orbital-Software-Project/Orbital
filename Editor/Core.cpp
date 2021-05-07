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


        // TODO ex:
        // wnd.AddView(taskpanel)
        // while(wnd.DoEvents()) {
        //  for(IView r : wnd.GetViews()) {
        //   ViewRequest req;
        //   if (r.HasRequest(&req)) {
        //    //Handle all requests from views in core instead of window class
        //   }
        //  }
        // }

    }
}


