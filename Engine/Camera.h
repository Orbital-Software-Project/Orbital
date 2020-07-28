#pragma once

#include "Mesh.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <memory>

namespace Orb {

class Camera {

public:
    Camera(std::shared_ptr<Shader> shader);

    ~Camera();

    const glm::mat4 GetViewMat();

    void SetViewMat(glm::mat4 view);

    void Draw();

private:
    glm::mat4 view;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;

};

}
