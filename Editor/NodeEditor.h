#pragma once

#include <memory>

#include <imgui.h>
#include <imgui_node_editor.h>

#include <vector>

namespace Orb {
    class SceneRenderer;
    class INode;
}

namespace Orb {

    struct LinkInfo
    {
        ax::NodeEditor::LinkId Id;
        ax::NodeEditor::PinId InputId;
        ax::NodeEditor::PinId OutputId;
    };

    // https://www.youtube.com/watch?v=xbTLhMJARrk&list=PLZSNHzwDCOggHLThIbCxUhWTgrKVemZkz
    // https://www.blenderfreak.com/tutorials/node-editor-tutorial-series/


    class NodeEditor {

    public:
        NodeEditor(std::shared_ptr<SceneRenderer> renderer);

        ~NodeEditor();

        void OnRender();

    private:
        void drawToolbar();

    private:
        std::shared_ptr<SceneRenderer> renderer;
        ax::NodeEditor::EditorContext* context = nullptr;

        ImVector<LinkInfo> links;
        std::vector<std::shared_ptr<INode>> nodes;

    };

}
