#pragma once

#include <memory>

#include "INode.h"
#include "Editor/Tasks/OpenVslamTask.h"

namespace Orb {

    class SlamNode : public INode {

    public:
        SlamNode();

        ~SlamNode();

        void OnRender(int& id);

    private:
        OpenVslamTask task;

    };
}