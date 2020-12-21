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

    void Draw(std::shared_ptr<Shader> shader);

    IEntity::EntityType GetEntityType() { return  IEntity::EntityType::Camera; }

    glm::vec3 CameraFront = glm::vec3(0.0f, 0.0f, 1.0f);

    glm::vec3 CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    float Yaw = 0;

    float Pitch = 0;

private:
    std::shared_ptr<Mesh> mesh;



};

}
