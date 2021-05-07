#include "NodeGraph.h"

#include <imnodes.h>
#include <imgui.h>
#include <cassert>

NodeGraph::NodeGraph() {
    imnodes::Initialize();
    
    imnodes::PushAttributeFlag(imnodes::AttributeFlags_EnableLinkDetachWithDragClick);

    imnodes::IO& io = imnodes::GetIO();
    io.link_detach_with_modifier_click.modifier = &ImGui::GetIO().KeyCtrl;
}

void NodeGraph::OnRender() {

    ImGui::Begin("Node editor");

    // Node editor
    {

        imnodes::BeginNodeEditor();

        for (auto node : this->nodes) {
            node->OnRender();
            if (node->RequestCompute) {
                this->doCompute = true;
                node->RequestCompute = false;
            }
        }

        // Link nodes
        for (int i = 0; i < this->links.size(); ++i)
        {
            const std::pair<int, int> p = this->links[i];
            // in this case, we just use the array index of the link
            // as the unique identifier
            imnodes::Link(i, p.first, p.second);
        }

        imnodes::EndNodeEditor();

        int start_attr, end_attr;
        if (imnodes::IsLinkCreated(&start_attr, &end_attr))
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
            this->pyramid.clear();
            std::vector<INode*> clip_nodes = this->GetBeginNodes();
            this->pyramid.push_back(clip_nodes);
            this->BuildNodePyramid(this->pyramid);
            this->doCompute = true;
            this->ComputeNodes();

        }
        int lnk_id;
        if (imnodes::IsLinkDestroyed(&lnk_id)) {
            this->links.erase(this->links.begin() + lnk_id);

            // Update node pyramid
            this->pyramid.clear();
            std::vector<INode*> clip_nodes = this->GetBeginNodes();
            this->pyramid.push_back(clip_nodes);
            this->BuildNodePyramid(this->pyramid);
            this->doCompute = true;
            this->ComputeNodes();
        }


    }

    // Node inspector
    {

        ImGui::Begin("Inspector");

        int num_selected_nodes = imnodes::NumSelectedNodes();
        if (num_selected_nodes > 0) {
            std::vector<int> s_nodes(num_selected_nodes);
            imnodes::GetSelectedNodes(s_nodes.data());

            int id = 0;
            for (int node_id : s_nodes) {
                ImGui::PushID(id++);
                {
                    if (this->GetNode(node_id)->OnRenderProperties()) {
                        ImGui::Separator();
                    }
                }
                ImGui::PopID();
            }
        }
        ImGui::End();

    }

    ImGui::End();
}

void NodeGraph::AddNode(INode* node)
{
	this->nodes.push_back(node);
}

std::vector<INode*> NodeGraph::GetBeginNodes() {
    std::vector<INode*> clips;
    for (auto nd : this->nodes) {
        if (nd->Type == INodeType::Node_Clip || nd->Type == INodeType::Node_FeatureTrack) {
            clips.push_back(static_cast<INode*>(nd));
        }
    }
    return clips;
}

void NodeGraph::SetFrameIdx(int frameIdx) {
    this->frameIdx = frameIdx;
}

INode* NodeGraph::GetNodeBySocket(int socket_id) {
    for (auto& node : this->nodes) {
        if (node->HasSocket(socket_id)) {
            return node;
        }
    }
    return nullptr;
}

std::vector<INode*> NodeGraph::GetParentNodes(INode* node) {
    auto input_sockets = node->GetInputSockets();
    std::vector<INode*> p_nodes;
    // get all input sockets of node and check if a node is connected to that
    for (auto &i_socket : input_sockets) {
        int i_socket_id = i_socket->SocketId;
        // check if link with input socket exist
        for (auto &link : this->links) {
            if (link.second == i_socket_id) {
                // link found, get parent node and add
                p_nodes.push_back(this->GetNodeBySocket(link.first));
            }
        }
    }
    return p_nodes;
}

void NodeGraph::BuildNodePyramid(std::vector<std::vector<INode*>> &nd_pyramid) {
    std::vector<INode*> parent_nodes_layer;

    for (auto bottom_layer : nd_pyramid[nd_pyramid.size() - 1]) {
        auto parent_nodes = this->GetParentNodes(bottom_layer);
        for (auto parent_nd : parent_nodes) {
            parent_nodes_layer.push_back(parent_nd);
        }
    }

    // if parent_nodes_layer has no items then the nodes have no parent connected nodes
    if (parent_nodes_layer.size() > 0) {
        nd_pyramid.push_back(parent_nodes_layer);

        this->BuildNodePyramid(nd_pyramid);
        this->doCompute = true;
    }
    

}

void NodeGraph::ResetNodes() {
    for (auto& nd : this->nodes) {
        nd->Reset();
    }
}

void NodeGraph::ComputeNodes() {

    static int last_frame_idx = -1;

    if (this->frameIdx != last_frame_idx || doCompute) {
        this->ResetNodes();
        this->doCompute = false;
    }
    
    // Reset computed status
    //this->ResetNodes();

    for (int i = static_cast<int>(this->pyramid.size()) -1; i >= 0; i--) {
        for (INode* parent_node : this->pyramid[i]) {
            
            if (!parent_node->IsComputed()) {
                parent_node->ComputeFrame(this->frameIdx);
            }

            // transfer data to next node
            for (auto &o_socket : parent_node->GetOutputSockets()) {
                for (std::pair<int, int> link : this->links) {
                    // find connected child node
                    if (link.first == o_socket->SocketId) {
                        INode* child_node = this->GetNodeBySocket(link.second);
                        std::shared_ptr<SocketData> parent_socket_data = parent_node->ReadSocket(link.first);

                        // Write data from parent to child ex. Video to clip node
                        child_node->WriteSocket(link.second, parent_socket_data);
                    }
                }
            }
        }
    }

    last_frame_idx = this->frameIdx;
    
}

void NodeGraph::ComputeClip(Clip* clip, int frameIdx) {
   
    if (frameIdx == -1) {
        frameIdx =  this->frameIdx;
    }

    // Compute pyramid from the bottom to the top (Clip nodes)
    for (int i = static_cast<int>(this->pyramid.size()) - 1; i >= 0; i--) {
        for (INode* parent : this->pyramid[i]) {

            // Top of pyramid
            if (i == 0) {
                if (parent->GetId() != clip->GetId()) {
                    continue;
                }
            }

            //if (!parent_node->IsComputed()) {
                parent->ComputeFrame(frameIdx);
            //}

            // transfer data to next node
            for (auto& o_socket : parent->GetOutputSockets()) {
                for (std::pair<int, int> link : this->links) {
                    // find connected child node
                    if (link.first == o_socket->SocketId) {
                        INode* child_node = this->GetNodeBySocket(link.second);

                        std::shared_ptr<SocketData> parent_socket_data = parent->ReadSocket(link.first);

                        // Write data from parent to child ex. Video to clip node
                        child_node->WriteSocket(link.second, parent_socket_data);
                    }
                }
            }
        }
    }





}

INode* NodeGraph::GetNode(int node_id) {
    auto it = std::find_if(this->nodes.begin(), this->nodes.end(), [&](INode* curr_node) {return curr_node->GetId() == node_id; });
    return *it;
}