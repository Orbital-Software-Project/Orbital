#pragma once

#include "Editor/Global.h"

#include "Editor/Views/ViewManager.h"

#include <string>


namespace Orb {

    class Core {

    public:
        Core(std::string rootDir);

        ~Core();

        void Run();

    private:
        std::string rootDir = "";
        

    };

}
