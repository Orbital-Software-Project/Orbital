#pragma once

#include "INode.h"

#include <memory>

namespace Orb {


class SlamNode : public INode {

public:
    SlamNode();

    ~SlamNode();

    void OnRender(int &id);

private:
    //std::unique_ptr<SlamTask> slamTask = nullptr;
};

}
