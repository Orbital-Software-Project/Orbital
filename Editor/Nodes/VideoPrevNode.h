#pragma once

#include "INode.h"


#include <string>

namespace Orb {

class VideoPrevNode : public INode {

public:
    VideoPrevNode();

    ~VideoPrevNode();

    void OnRender(int &id);

};

}
