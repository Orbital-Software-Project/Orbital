#pragma once

#include <memory>
#include "SceneRenderer.h"


#include "imgui_node_editor.h"


namespace Orb {

namespace ed = ax::NodeEditor;


class NodeEditor {

public:
    NodeEditor(std::shared_ptr<SceneRenderer> renderer);

    ~NodeEditor();

    void OnRender();

private:
    std::shared_ptr<SceneRenderer> renderer;
    ax::NodeEditor::EditorContext* g_Context = nullptr;

};

}
