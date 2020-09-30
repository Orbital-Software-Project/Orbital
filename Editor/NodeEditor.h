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


class NodeEditor {

public:
    NodeEditor(std::shared_ptr<SceneRenderer> renderer);

    ~NodeEditor();

    void OnRender();

private:
    std::shared_ptr<SceneRenderer> renderer;
    ax::NodeEditor::EditorContext* context = nullptr;

    ImVector<LinkInfo> links;
    std::vector<std::shared_ptr<INode>> nodes;

};

}
