#pragma once

#include "INode.h"


#include <string>

namespace Orb {

class FileNode : public INode {

public:
    FileNode();

    ~FileNode();

    void OnRender(int &id);


};

}
