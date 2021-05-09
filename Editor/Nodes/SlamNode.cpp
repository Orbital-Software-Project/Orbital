#include "SlamNode.h"

//#include "../SlamTask.h"

#include <imgui_node_editor.h>

#include <imnodes.h>

namespace Orb {

    SlamNode::SlamNode() {

	}

    SlamNode::~SlamNode() {

	}

	void SlamNode::OnRender(int &id) {

        ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(224, 112, 106, 255));

        ImNodes::BeginNode(id++);
        {
            
            ImNodes::BeginNodeTitleBar();
            {
                ImGui::Text("Track and Map");
            }
            ImNodes::EndNodeTitleBar();

            ImGui::Dummy(ImVec2(150.0f, 0.0f));

            {
                ImNodes::BeginInputAttribute(id++);
                ImGui::Text("Video");
                ImNodes::EndInputAttribute();

                ImGui::Spacing();

                ImNodes::BeginInputAttribute(id++);
                ImGui::Text("Config");
                ImNodes::EndInputAttribute();

                ImGui::Spacing();

                ImNodes::BeginInputAttribute(id++);
                ImGui::Text("Orb Vocab");
                ImNodes::EndInputAttribute();


                if (ImGui::Button("Start")) {

                }
            }

        }
        ImNodes::EndNode();

	}

}
