#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

namespace Orb {

class Shader {

public:
    Shader(std::string vertex, std::string fragment);
    ~Shader();

    void Use();

private:
    unsigned int programID;

};

}
