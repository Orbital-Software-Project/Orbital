#include "NodeEditor.h"
#include "SceneRenderer.h"

#include "Nodes/INode.h"
#include "Nodes/FileNode.h"
#include "Nodes/SlamNode.h"

namespace Orb {

NodeEditor::NodeEditor(std::shared_ptr<SceneRenderer> renderer) {
    this->renderer = renderer;
    this->context = ax::NodeEditor::CreateEditor();

    // Default node setup
    this->nodes.push_back(std::make_shared<FileNode>());
    this->nodes.push_back(std::make_shared<SlamNode>());

}

NodeEditor::~NodeEditor() {

    ax::NodeEditor::DestroyEditor(this->context);
}

void NodeEditor::OnRender() {

    ImGui::Begin("Node Editor");
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
                int linkId = 100;
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
    ImGui::End();

    return;


}

}
