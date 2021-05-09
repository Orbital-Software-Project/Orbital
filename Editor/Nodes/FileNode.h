#pragma once

#include "INode.h"


#include <string>

namespace Orb {

class FileNode : public INode {

public:
    FileNode();

    ~FileNode();

    void OnRender(int &id);

private:
    char charBuf[1000] = { '\0' };


};

}
