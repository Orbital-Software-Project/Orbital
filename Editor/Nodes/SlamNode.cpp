#include "SlamNode.h"

#include "../SlamTask.h"

#include <imgui_node_editor.h>


namespace Orb {

    SlamNode::SlamNode() {

	}

    SlamNode::~SlamNode() {

	}

	void SlamNode::OnRender(int &id) {

        ax::NodeEditor::BeginNode(id++);
        {
            ImGui::Text("Track and Map");
            {
                ax::NodeEditor::BeginPin(id++, ax::NodeEditor::PinKind::Input);
                ImGui::Text("<- Video ");
                ax::NodeEditor::EndPin();

                ImGui::SameLine();

                ax::NodeEditor::BeginPin(id++, ax::NodeEditor::PinKind::Output);
                ImGui::Text("3d Map ->");
                ax::NodeEditor::EndPin();


                ax::NodeEditor::BeginPin(id++, ax::NodeEditor::PinKind::Input);
                ImGui::Text("<- Config ");
                ax::NodeEditor::EndPin();

                ImGui::SameLine();

                ax::NodeEditor::BeginPin(id++, ax::NodeEditor::PinKind::Output);
                ImGui::Text("Curr. Frame ->");
                ax::NodeEditor::EndPin();

                ax::NodeEditor::BeginPin(id++, ax::NodeEditor::PinKind::Input);
                ImGui::Text("<- Orb Vocab ");
                ax::NodeEditor::EndPin();

                
                

                if (ImGui::Button("Start")) {

                }
                ImGui::SameLine();
                if (ImGui::Button("Pause")) {

                }
                ImGui::SameLine();
                if (ImGui::Button("Stop")) {

                }

            }
        }
        ax::NodeEditor::EndNode();

	}

}
