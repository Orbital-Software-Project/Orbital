#pragma once

#include "Engine/SceneRenderer.h"

#include "Editor/Views/IView.hpp"

namespace Orb {

class Outliner : public IView {

public:
    Outliner(std::shared_ptr<SceneRenderer> renderer);

    ~Outliner();

    void OnRender();

private:
    std::shared_ptr<SceneRenderer> renderer = nullptr;

};

}
