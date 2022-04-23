#pragma once

#include "Mesh.h"

namespace Orb {

class PrimitiveFactory {

public:
    PrimitiveFactory();

    ~PrimitiveFactory();

    static std::shared_ptr<Mesh> Cube();

    static std::shared_ptr<Mesh> Grid();

    static std::shared_ptr<Mesh> Plane();

    static std::shared_ptr<Mesh> SizedPlane(float width, float height);


};

}
