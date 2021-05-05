#pragma once

#include <string>
#include "Editor/Global.h"

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
