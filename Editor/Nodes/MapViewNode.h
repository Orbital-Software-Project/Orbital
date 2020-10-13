#pragma once

#include "INode.h"


#include <string>

namespace Orb {

class MapViewNode : public INode {

public:
    MapViewNode();

    ~MapViewNode();

    void OnRender(int &id);

};

}
