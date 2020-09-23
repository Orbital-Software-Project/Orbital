#include "NodeEditor.h"


#include <imgui.h>






namespace Orb {



NodeEditor::NodeEditor(std::shared_ptr<SceneRenderer> renderer) {
    this->renderer = renderer;

    
    this->g_Context = ax::NodeEditor::CreateEditor();


}

NodeEditor::~NodeEditor() {

    ax::NodeEditor::DestroyEditor(this->g_Context);
}

void NodeEditor::OnRender() {

    


    ImGui::Begin("Node Editor");
    ed::SetCurrentEditor(g_Context);

    ed::Begin("NodeEd");

    int uniqueId = 1;

    // Start drawing nodes.
    ed::BeginNode(uniqueId++);
    {
        ImGui::Text("Node A");
        {
            ed::BeginPin(uniqueId++, ed::PinKind::Input);
            ImGui::Text("-> In");
            ed::EndPin();

            ImGui::SameLine();
            ed::BeginPin(uniqueId++, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();

            ImGui::Button("test");
        }
        ed::EndNode();
    }
    ed::End();


    ImGui::End();

}

}
