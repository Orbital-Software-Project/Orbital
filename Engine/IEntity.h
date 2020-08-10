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

    virtual void SetVisible(bool visible) = 0;

    virtual bool IsVisible() = 0;

    virtual glm::mat4 GetMatrix() = 0;

    virtual void SetMatrix(glm::mat4 matrix) = 0;

};

}
