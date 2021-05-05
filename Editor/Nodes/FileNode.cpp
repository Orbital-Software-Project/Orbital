#include "FileNode.h"

#include "../Utils.hpp"


#include <imgui_node_editor.h>

#include <nfd.h>


namespace Orb {

	FileNode::FileNode() {

	}

	FileNode::~FileNode() {

	}

	void FileNode::OnRender(int &id) {

        ax::NodeEditor::BeginNode(id++);
        {
            ImGui::Text("File");
            {
                ax::NodeEditor::BeginPin(id++, ax::NodeEditor::PinKind::Output);
                ImGui::Text("Out ->");
                ax::NodeEditor::EndPin();

                static char charBuf[1000];
                ImGui::SetNextItemWidth(150);
                ImGui::InputText("", charBuf, 1000);

                ImGui::SameLine();

                if (ImGui::Button("...")) {
                    strcpy_s(charBuf, Utils::PickFile().c_str());                
                }
            }
        }
        ax::NodeEditor::EndNode();

	}

}
