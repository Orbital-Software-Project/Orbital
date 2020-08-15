#pragma once

#include "Mesh.h"
#include "Camera.h"
#include "SceneRenderer.h"

#include <memory>

namespace Orb {

class PropertyEditor {

public:
    PropertyEditor(std::shared_ptr<SceneRenderer> renderer);

    ~PropertyEditor();

    void OnRender();

private:
    void matrix4x4(glm::mat4 mat);

    std::shared_ptr<SceneRenderer> renderer;

};

}
