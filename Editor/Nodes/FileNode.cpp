#include "FileNode.h"

#include "../Utils.hpp"


#include <nfd.h>
#include <imgui.h>

#include <imnodes.h>

namespace Orb {

	FileNode::FileNode() {

	}

	FileNode::~FileNode() {

	}

    void FileNode::OnRender(int& id) {

        ImNodes::PushColorStyle(ImNodesCol_TitleBar, IM_COL32(111, 175, 223, 255));

        ImNodes::BeginNode(id++);
        {

            ImNodes::BeginNodeTitleBar();
            {
                ImGui::Text("File");
            }
            ImNodes::EndNodeTitleBar();

            {
                ImNodes::BeginOutputAttribute(id++);
                ImGui::Text("Out");
                ImNodes::EndOutputAttribute();


                ImGui::SetNextItemWidth(150);
                ImGui::InputText("", this->charBuf, 1000);

                ImGui::SameLine();

                if (ImGui::Button("...")) {
                    strcpy_s(this->charBuf, Utils::PickFile().c_str());
                }
            }
        }
        ImNodes::EndNode();

    }

}
