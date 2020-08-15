#pragma once

#include "IEntity.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <memory>

namespace Orb {

class Camera : public IEntity {

public:
    Camera();

    ~Camera();

    virtual void Draw(std::shared_ptr<Shader> shader);

private:
    std::shared_ptr<Mesh> mesh;

};

}
