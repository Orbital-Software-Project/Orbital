#pragma once

#include "Shader.h"

#include <memory>
#include <glm/glm.hpp>

namespace Orb {

class IEntity {

public:
    IEntity() {};

    virtual ~IEntity() {}

public:
    virtual void Draw(std::shared_ptr<Shader> shader) = 0;

    bool Selected = false;

    bool Visible = true;

    glm::mat4 Matrix = glm::mat4(1.0f);

    glm::vec3 Position = glm::vec4(0.0f);

    glm::vec3 Rotation = glm::vec4(0.0f);

    glm::vec3 Scale = glm::vec4(1.0f);

    std::string Name = "IEntity";

};

}
