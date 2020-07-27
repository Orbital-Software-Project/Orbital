#include "VideoPreview.h"
#include "Global.h"


#include <imgui.h>

#include <stdint.h>
#include <iostream>

#include <GL/glew.h>

#include <string>


namespace Orb {

VideoPreview::VideoPreview() {}

VideoPreview::~VideoPreview() {}

void VideoPreview::OnRender(){
    ImGui::Begin("Video preview");

    //ImGui::Text("Frames %i / %i", Global::getInstance().FramesProcessed, Global::getInstance().NumFrames);

    if(Global::getInstance().FramePublisher.get() != nullptr) {

        this->videoFrame.UpdateColorMap(Global::getInstance().FramePublisher->draw_frame());

        // Get content size of the imgui window
        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

        // get current size of the imgui window
        ImVec2 vSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y - 50);


        ImGui::Image((void*)(intptr_t)this->videoFrame.GetTextureID(), vSize);


    }
    //ImGui::Text(Global::CurrentVideoFile.c_str());

    ImGui::End();
}

}
