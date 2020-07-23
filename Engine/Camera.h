#pragma once

#include "Mesh.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <memory>

namespace Orb {

class Camera {

public:
    Camera();

    ~Camera();

    const glm::mat4 GetViewMat();

    void SetViewMat(glm::mat4 view);

    void Draw(std::shared_ptr<Shader> shader);

private:
    glm::mat4 view;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;

};

}
