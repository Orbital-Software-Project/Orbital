#pragma once

#include <string>
#include <Eigen/Core>

namespace Orb {

class Shader {

public:
    Shader(std::string vertex, std::string fragment);

    ~Shader();

    void Use();

    void SetVec4(std::string uniform, Eigen::Vector4f value);

private:
    unsigned int programID;


};

}
