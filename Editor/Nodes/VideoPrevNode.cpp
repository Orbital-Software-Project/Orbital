#include "VideoPrevNode.h"

#include "../Utils.hpp"


#include <nfd.h>

#include <imgui.h>

namespace Orb {

    VideoPrevNode::VideoPrevNode() {

	}

    VideoPrevNode::~VideoPrevNode() {

	}

	void VideoPrevNode::OnRender(int &id) {

        //ax::NodeEditor::BeginNode(id++);
        {
            ImGui::Text("Video Preview");
            {

                {
                    //ax::NodeEditor::BeginPin(id++, //ax::NodeEditor::PinKind::Input);
                    ImGui::Text("<- File");
                    //ax::NodeEditor::EndPin();
                }

                ImGui::SameLine();

                {
                    //ax::NodeEditor::BeginPin(id++, //ax::NodeEditor::PinKind::Output);
                    ImGui::Text("Video ->");
                    //ax::NodeEditor::EndPin();
                }
                


            }
        }
        //ax::NodeEditor::EndNode();

	}

}
