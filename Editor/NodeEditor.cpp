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
        ImGui::Text("Video");
        {
            ImGui::SameLine();
            ed::BeginPin(uniqueId++, ed::PinKind::Output);
            ImGui::Text("Out ->");
            ed::EndPin();

            static char charBuf[255];
            ImGui::SetNextItemWidth(100);
            ImGui::InputText("", charBuf, 255);

            ImGui::SameLine();

            ImGui::Button("...");
        }
        ed::EndNode();
    }
    ed::End();


    ImGui::End();

}

}
