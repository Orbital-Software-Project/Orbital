#include "NodeEditor.h"
#include "Engine/SceneRenderer.h"

#include "Editor/Nodes/INode.h"
#include "Editor/Nodes/FileNode.h"
#include "Editor/Nodes/SlamNode.h"
#include "Editor/Nodes/VideoPrevNode.h"
#include "Editor/Nodes/MapViewNode.h"

#include <imnodes_internal.h>

namespace Orb {


NodeEditor::NodeEditor(std::shared_ptr<SceneRenderer> renderer) {
    this->renderer = renderer;

    this->Name = "Node editor";

    // Default node setup
    this->nodes.push_back(std::make_shared<FileNode>());
    this->nodes.push_back(std::make_shared<FileNode>());
    this->nodes.push_back(std::make_shared<FileNode>());
    this->nodes.push_back(std::make_shared<SlamNode>());


    this->context = ImNodes::CreateContext();

    ImNodes::PushAttributeFlag(ImNodesAttributeFlags_EnableLinkDetachWithDragClick);

    auto& io = ImNodes::GetIO();
    io.LinkDetachWithModifierClick.Modifier = &ImGui::GetIO().KeyCtrl;

}


NodeEditor::~NodeEditor() {

}

void NodeEditor::OnRender() {
    this->drawToolbar();

    // Node editor
    {

        ImNodes::BeginNodeEditor();

        int id = 0;
        for (auto node : this->nodes) {
            node->OnRender(id);
        }

        // Link nodes
        for (int i = 0; i < this->links.size(); ++i)
        {
            const std::pair<int, int> p = this->links[i];
            // in this case, we just use the array index of the link
            // as the unique identifier
            ImNodes::Link(i, p.first, p.second);
        }

        ImNodes::MiniMap(0.09f, ImNodesMiniMapLocation_TopRight);

        ImNodes::EndNodeEditor();


        int start_attr, end_attr;
        if (ImNodes::IsLinkCreated(&start_attr, &end_attr))
        {

            // TODO: Check if input socket is already used
            // TODO: Check if circular connection exists

            // check if socket is already used
            for (auto& lnk : this->links) {
                if (lnk.second == end_attr) {
                    return;
                }
            }

            links.push_back(std::make_pair(start_attr, end_attr));

            // Update node pyramid

        }
        int lnk_id = 0;
        if (ImNodes::IsLinkDestroyed(&lnk_id)) {
            this->links.erase(this->links.begin() + lnk_id);
        }


    }


}


void NodeEditor::drawToolbar() {
    
    const char* items[] = { 
        "File Node", 
        "Track and Map Tasknode",
        "Map Viewer Node",
        "Video preview Node",
    };

    static const char* current_item = NULL;

    ImGui::SetNextItemWidth(200);
    if (ImGui::BeginCombo("##combo", "Add Node")) {

        for (int n = 0; n < IM_ARRAYSIZE(items); n++)
        {
            bool is_selected = (current_item == items[n]);
            if (ImGui::Selectable(items[n], is_selected)) {
                current_item = items[n];
                switch (n) {
                case 0:
                    this->nodes.push_back(std::make_shared<FileNode>());
                    break;
                case 1:
                    this->nodes.push_back(std::make_shared<SlamNode>());
                    break;
                case 2:
                    break;
                case 3:
                    break;
                }

            }

            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }

        }

        ImGui::EndCombo();
    }
    

    ImGui::Separator();

}
}
