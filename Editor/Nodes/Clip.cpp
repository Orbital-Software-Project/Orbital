#include "clip.h"

#include <imgui.h>
#include <imnodes.h>

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

Clip::Clip() {
    this->Type = INodeType::Node_Clip;

    this->video_input_id = this->RegisterInputSocket(INodeDataType::Type_OpenCV_Mat);

}

Clip::~Clip() {

}

void Clip::OnRender() {
    imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(0, 150, 0, 255));

    imnodes::BeginNode(this->id);
    {
        imnodes::BeginNodeTitleBar();
        {
            ImGui::Text("Sequencer clip");
        }
        imnodes::EndNodeTitleBar();

        imnodes::BeginInputAttribute(this->video_input_id->SocketId);
        {
            ImGui::Text("Out");
        }
        imnodes::EndOutputAttribute();
    }
    imnodes::EndNode();

    imnodes::PopColorStyle();
}

bool Clip::OnRenderProperties() {

    if (ImGui::Button("Add to Sequencer")) {
        this->AddToSequencer = true;
    }

    return true;
}

bool Clip::ComputeFrame(int frameIdx) {
    auto socket = this->ReadSocket(this->video_input_id->SocketId);

    if (socket.get() != nullptr && !socket->NodeType_Video.MatData.empty()) {
        //this->createTexture(socket->NodeType_Video.MatData);

        this->image = socket->NodeType_Video.MatData;
        this->frame_count = socket->NodeType_Video.FrameCount;

    } else {

        this->image = this->outChessboard.clone();
        //this->createTexture(this->outChessboard);
    }

    this->isComputed = true;
    return true;
}
