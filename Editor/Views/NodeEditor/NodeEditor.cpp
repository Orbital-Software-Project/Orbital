#include "NodeEditor.h"
#include "Engine/SceneRenderer.h"

#include "Editor/Nodes/INode.h"
#include "Editor/Nodes/FileNode.h"
#include "Editor/Nodes/SlamNode.h"
#include "Editor/Nodes/VideoPrevNode.h"
#include "Editor/Nodes/MapViewNode.h"

namespace Orb {

NodeEditor::NodeEditor(std::shared_ptr<SceneRenderer> renderer) {
    this->renderer = renderer;
    this->context = ax::NodeEditor::CreateEditor();

    // Default node setup
    this->nodes.push_back(std::make_shared<FileNode>());
    this->nodes.push_back(std::make_shared<FileNode>());
    this->nodes.push_back(std::make_shared<FileNode>());

    this->nodes.push_back(std::make_shared<VideoPrevNode>());
    this->nodes.push_back(std::make_shared<SlamNode>());
    this->nodes.push_back(std::make_shared<MapViewNode>());


}


NodeEditor::~NodeEditor() {

    ax::NodeEditor::DestroyEditor(this->context);
}

void NodeEditor::OnRender() {

    this->drawToolbar();
    {
        ax::NodeEditor::SetCurrentEditor(context);
        {
            ax::NodeEditor::Begin("NodeEd");
            {

                int id = 1;
                for (std::shared_ptr<INode> node : this->nodes) {
                    node->OnRender(id);
                    id += 1;
                }

                // Build links
                for (auto& linkInfo : this->links) {
                    ax::NodeEditor::Link(linkInfo.Id, linkInfo.InputId, linkInfo.OutputId);
                }


                /*ax::NodeEditor::Suspend();
                {
                    static ax::NodeEditor::NodeId contextNodeId = 0;

                    if (ax::NodeEditor::ShowBackgroundContextMenu()) {
                        ImGui::OpenPopup("Node Context Menu");
                    }
                    ax::NodeEditor::Resume();

                    ax::NodeEditor::Suspend();

                    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
                    if (ImGui::BeginPopup("Create New Node"))
                    {
                        ImGui::MenuItem("Input Action");

                        ImGui::MenuItem("Output Action");


                        ImGui::EndPopup();
                    }
                }
                ax::NodeEditor::Resume();*/


                // Create link logic
                static int linkId = 0;
                if (ax::NodeEditor::BeginCreate())
                {
                    ax::NodeEditor::PinId inputPinId, outputPinId;
                    if (ax::NodeEditor::QueryNewLink(&inputPinId, &outputPinId))
                    {

                        if (ax::NodeEditor::AcceptNewItem())
                        {
                            // Since we accepted new link, lets add one to our list of links.
                            this->links.push_back({ ax::NodeEditor::LinkId(linkId++), inputPinId, outputPinId });

                            // Draw new link.
                            ax::NodeEditor::Link(this->links.back().Id, this->links.back().InputId, this->links.back().OutputId);
                        }

                    }
                }
                ax::NodeEditor::EndCreate();


                // Delete logic
                if (ax::NodeEditor::BeginDelete())
                {
                    ax::NodeEditor::LinkId deletedLinkId;
                    while (ax::NodeEditor::QueryDeletedLink(&deletedLinkId))
                    {
                        if (ax::NodeEditor::AcceptDeletedItem())
                        {
                            for (auto& link : this->links)
                            {
                                if (link.Id == deletedLinkId)
                                {
                                    this->links.erase(&link);
                                    break;
                                }
                            }
                        }
                    }
                }
                ax::NodeEditor::EndDelete();
            }
            ax::NodeEditor::End();
        }
        ax::NodeEditor::SetCurrentEditor(nullptr);
    }


    return;
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
