#pragma once

#include "SceneRenderer.h"

namespace Orb {

class Outliner {

public:
    Outliner(std::shared_ptr<SceneRenderer> renderer);

    ~Outliner();

    void OnRender();

private:
    std::shared_ptr<SceneRenderer> renderer = nullptr;

};

}
