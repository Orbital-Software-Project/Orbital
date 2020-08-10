#pragma once

#include "IEntity.h"
#include "Mesh.h"

#include <glm/glm.hpp>
#include <memory>

namespace Orb {

struct CameraData {
    glm::mat4 ModelViewMat;
};

class Camera : public IEntity {

public:
    Camera();

    ~Camera();

    virtual void Draw(std::shared_ptr<Shader> shader);

    virtual void SetVisible(bool visible);

    virtual bool IsVisible();

    virtual void SetMatrix(glm::mat4 matrix);

    virtual glm::mat4 GetMatrix();

private:
    CameraData data;
    std::shared_ptr<Mesh> mesh;
    bool visible = true;

};

}
