#pragma once


#include <imgui.h>


#include "Editor/Views/IView.hpp"
#include "Engine/Texture.h"


#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <utility>
#include <memory>


#include <imnodes.h>

namespace Orb {
    class SceneRenderer;
    class INode;
}

namespace Orb {

    // https://www.youtube.com/watch?v=xbTLhMJARrk&list=PLZSNHzwDCOggHLThIbCxUhWTgrKVemZkz
    // https://www.blenderfreak.com/tutorials/node-editor-tutorial-series/

    class NodeEditor : public IView {

    public:
        NodeEditor(std::shared_ptr<SceneRenderer> renderer);

        ~NodeEditor();

        void OnRender();

    private:
        void drawToolbar();


    private:
        std::shared_ptr<SceneRenderer> renderer;

        std::vector<std::pair<int, int>> links;
        std::vector<std::shared_ptr<INode>> nodes;

        ImNodesContext* context;

    };

}
