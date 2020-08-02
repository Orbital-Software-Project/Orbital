#pragma once

#include "Mesh.h"
#include "Shader.h"

#include <glm/glm.hpp>
#include <memory>

namespace Orb {

struct CameraData {
    glm::mat4 ModelViewMat;
};

class Camera {

public:
    Camera(std::shared_ptr<Shader> shader);

    ~Camera();

    const glm::mat4 GetViewMat();

    void SetViewMat(glm::mat4 view);

    void Draw();

    void ToggleHide();

private:
    CameraData data;
    std::shared_ptr<Mesh> mesh;
    std::shared_ptr<Shader> shader;

    bool visible = true;
};

}
