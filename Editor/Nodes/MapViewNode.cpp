#include "MapViewNode.h"
#include "../Utils.hpp"


#include <imgui_node_editor.h>


namespace Orb {

    MapViewNode::MapViewNode() {

	}

    MapViewNode::~MapViewNode() {

	}

	void MapViewNode::OnRender(int &id) {
        ax::NodeEditor::BeginNode(id++);
        {
            ImGui::Text("Map Viewer");
            {
                {
                    ax::NodeEditor::BeginPin(id++, ax::NodeEditor::PinKind::Input);
                    ImGui::Text("<- Map");
                    ax::NodeEditor::EndPin();
                }
            }
        }
        ax::NodeEditor::EndNode();

	}

}
