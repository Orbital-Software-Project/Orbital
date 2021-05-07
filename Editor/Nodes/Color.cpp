#include "Color.h"

#include <imgui.h>
#include <imnodes.h>

#include <iostream>

#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>

Color::Color() {
    this->Type = INodeType::Node_Color;

    this->ns_input  = this->RegisterInputSocket(INodeDataType::Type_OpenCV_Mat);
    this->ns_output = this->RegisterOutputSocket(INodeDataType::Type_OpenCV_Mat);

}

Color::~Color() {

}

void Color::OnRender() {

    imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(150, 0, 0, 255));


    imnodes::BeginNode(this->id);
    {
        imnodes::BeginNodeTitleBar();
        {
            ImGui::Text("Color Threshold");
        }
        imnodes::EndNodeTitleBar();

        imnodes::BeginInputAttribute(this->ns_input->SocketId);
        {
            ImGui::Text("Input");
        }
        imnodes::EndOutputAttribute();


        imnodes::BeginOutputAttribute(this->ns_output->SocketId);
        {
            ImGui::Text("Out");
        }
        imnodes::EndOutputAttribute();
    }
    imnodes::EndNode();

    imnodes::PopColorStyle();

}

bool Color::OnRenderProperties() {

    ImGui::SetNextItemWidth(150);
    this->RequestCompute |= ImGui::SliderInt("R", &this->slider_R, 0, 255);

    ImGui::SetNextItemWidth(150);
    this->RequestCompute |= ImGui::SliderInt("G", &this->slider_G, 0, 255);

    ImGui::SetNextItemWidth(150);
    this->RequestCompute |= ImGui::SliderInt("B", &this->slider_B, 0, 255);

    return true;
}

bool Color::ComputeFrame(int frameIdx) {
    std::shared_ptr<SocketData> input = this->ReadSocket(this->ns_input->SocketId);


    if (input.get() != nullptr && !input->NodeType_Video.MatData.empty()) {
        std::shared_ptr<SocketData> output = std::make_shared< SocketData>();

        output->NodeType_Video = input->NodeType_Video;
        output->NodeType_Video.MatData = input->NodeType_Video.MatData.clone();

        typedef cv::Point3_<uint8_t> Pixel;

        output->NodeType_Video.MatData.forEach<Pixel>
            (
                [&](Pixel& pixel, const int* position) -> void
                {
                    pixel.x = pixel.x <= this->slider_B ? pixel.x : 0;
                    pixel.y = pixel.y <= this->slider_G ? pixel.y : 0;
                    pixel.z = pixel.z <= this->slider_R ? pixel.z : 0;
                }
        );

        this->WriteSocket(this->ns_output->SocketId, output);
    }


    this->isComputed = true;

    return this->isComputed;
}