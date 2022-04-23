#pragma once

#include "Editor/Engine/Mesh.h"
#include "Editor/Engine/Camera.h"
#include "Editor/Engine/SceneRenderer.h"

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
