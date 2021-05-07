#include "FeatureTrack.h"

#include <imgui.h>
#include <imnodes.h>

#include <algorithm>
#include <execution>

#include <map>

#include <ImThread.h>

FeatureTrack::FeatureTrack() {
    this->Type = INodeType::Node_FeatureTrack;

    this->video_input = this->RegisterInputSocket(INodeDataType::Type_OpenCV_VideoCapture);


    this->akaze = cv::AKAZE::create();
    this->akaze->setThreshold(akaze_thresh);


    //auto test = std::bind(&FeatureTrack::doTrack, this);

   // void(Func)();

    //std::function<void()> track_func = std::bind(&FeatureTrack::doTrack, this);
    
    //Func = track_func;

    //ImThread::DefineThread("doTrack", &track_func);
    
    //std::bind(&FeatureTrack::doTrack, this);
    
}

FeatureTrack::~FeatureTrack() {

}

void FeatureTrack::OnRender() {

    imnodes::PushColorStyle(imnodes::ColorStyle_TitleBar, IM_COL32(0, 150, 0, 255));


    imnodes::BeginNode(this->id);
    {
        imnodes::BeginNodeTitleBar();
        {
            ImGui::Text("Feature tracking");
        }
        imnodes::EndNodeTitleBar();

        imnodes::BeginInputAttribute(this->video_input->SocketId);
        {
            ImGui::Text("Input");
        }
        imnodes::EndOutputAttribute();


    }
    imnodes::EndNode();

    imnodes::PopColorStyle();

}

bool FeatureTrack::OnRenderProperties() {

    if (ImGui::Button("Do Track")) {

    }

    return true;
}

bool FeatureTrack::ComputeFrame(int frameIdx) {



    return true;
}

void FeatureTrack::doTrack() {

    auto sc_data = this->ReadSocket(this->video_input->SocketId);
    if (sc_data->StrData.compare("") == 0) {
        return;
    }

    cv::VideoCapture vc = cv::VideoCapture(sc_data->StrData);

    cv::Mat curr_frame;
    struct KP_Desc {
        cv::Mat Desc = cv::Mat();
        std::vector<cv::KeyPoint> Kp;
        int FrameIdx = 0;
    };

    std::map<int, KP_Desc> data;
    while (vc.read(curr_frame)) {
        KP_Desc res;

        res.FrameIdx = res.FrameIdx++;

        this->akaze->detectAndCompute(sc_data->NodeType_Video.MatData, cv::Mat(), res.Kp, res.Desc);

        data.emplace(res.FrameIdx, res);
    }

    this->isComputed = true;

}