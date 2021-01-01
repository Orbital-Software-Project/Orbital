#pragma once

#include "Engine/Mesh.h"
#include "Engine/Camera.h"
#include "Engine/SceneRenderer.h"

#include "Editor/Views/IView.hpp"

#include <memory>

namespace Orb {

class PropertyEditor : public IView {

public:
    PropertyEditor(std::shared_ptr<SceneRenderer> renderer);

    ~PropertyEditor();

    void OnRender();

private:
    void matrix4x4(glm::mat4 mat);

    std::shared_ptr<SceneRenderer> renderer;

};

}
