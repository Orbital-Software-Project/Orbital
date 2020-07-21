#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Orb {

class Shader {

public:
    Shader(std::string vertex, std::string fragment);

    ~Shader();

    void Use();

    void SetVec4(std::string uniform, glm::vec4 value);

    void SetMat4(std::string uniform, glm::mat4 value);

private:
    unsigned int programID;


};

}
