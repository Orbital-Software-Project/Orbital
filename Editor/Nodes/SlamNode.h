#pragma once

#include "INode.h"

namespace Orb {

class SlamNode : public INode {

public:
    SlamNode();

    ~SlamNode();

    void OnRender(int &id);

};

}
