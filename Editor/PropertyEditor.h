#pragma once

#include "Mesh.h"
#include "Camera.h"

#include <memory>

namespace Orb {

class PropertyEditor {

public:
    PropertyEditor();

    ~PropertyEditor();

    void OnRender();

private:
    void matrix4x4(glm::mat4 mat);

};

}
