#pragma once


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
