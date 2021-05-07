#include "video.h"

#include <imgui.h>
#include <imnodes.h>

#include <iostream>

#include <opencv2/imgcodecs.hpp>

Video::Video() {
    this->Type = INodeType::Node_Video;

    this->video_output = this->RegisterOutputSocket(INodeDataType::Type_OpenCV_Mat);

    this->filePath = "C:\\Users\\rphil\\Desktop\\Brainstorm.mp4";
    this->video = cv::VideoCapture(this->filePath);

    this->frameCount = static_cast<int>(this->video.get(cv::CAP_PROP_FRAME_COUNT));
    this->width      = static_cast<int>(this->video.get(cv::CAP_PROP_FRAME_WIDTH));
    this->height     = static_cast<int>(this->video.get(cv::CAP_PROP_FRAME_HEIGHT));


    if (!this->video.isOpened()) {
        std::cout << "Video is not open" << std::endl;
    }


}

Video::~Video() {

}

void Video::OnRender() {

    imnodes::BeginNode(this->id);
    {
        imnodes::BeginNodeTitleBar();
        {
            ImGui::Text("Video");
        }
        imnodes::EndNodeTitleBar();

        imnodes::BeginOutputAttribute(this->video_output->SocketId);
        {
            ImGui::Text("Out");
        }
        imnodes::EndOutputAttribute();
    }
    imnodes::EndNode();

}

bool Video::OnRenderProperties() { 

    static char fileBuf[500];
    if (ImGui::InputText("File", this->filePath.data(), this->filePath.size(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly)) {

    }
    ImGui::SameLine();
    if (ImGui::Button("...")) {
        //if (ImFile::Pick("(.exe)|*.exe", ImFileOption::SingleFile)) {

        this->video = cv::VideoCapture(this->filePath);

        this->frameCount = static_cast<int>(this->video.get(cv::CAP_PROP_FRAME_COUNT));
        this->width      = static_cast<int>(this->video.get(cv::CAP_PROP_FRAME_WIDTH));
        this->height     = static_cast<int>(this->video.get(cv::CAP_PROP_FRAME_HEIGHT));


        //}
    }

    return true;
}

bool Video::ComputeFrame(int frameIdx) {

    std::shared_ptr<SocketData> data = std::make_shared<SocketData>();

    data->NodeType_Video.Video = this->video;
    data->NodeType_Video.FrameCount = this->frameCount;
    data->NodeType_Video.FilePath = this->filePath;

    if (frameIdx < this->frameCount) {
        this->video.set(cv::CAP_PROP_POS_FRAMES, frameIdx);
        this->video.read(this->curr_frame);
        data->NodeType_Video.MatData = curr_frame.clone();

    } else {
        data->NodeType_Video.MatData = cv::Mat::zeros(this->curr_frame.rows, this->curr_frame.cols, this->curr_frame.type());
    }

    this->WriteSocket(this->video_output->SocketId, data);

    this->isComputed = true;

    return this->isComputed;
}