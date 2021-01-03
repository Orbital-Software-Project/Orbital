#pragma once

#include "Engine/SceneRenderer.h"

#include "Editor/Views/IView.hpp"
#include "Engine/EntityGroup.hpp"

namespace Orb {

class Outliner : public IView {

public:
    Outliner(std::shared_ptr<SceneRenderer> renderer);

    ~Outliner();

    void OnRender();

private:
    void drawEntityTree(int id, std::vector<std::shared_ptr<IEntity>> entities);

private:
    std::shared_ptr<SceneRenderer> renderer = nullptr;

};

}
