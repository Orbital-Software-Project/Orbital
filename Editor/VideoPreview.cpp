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

    ImGui::Text("Frames %i / %i", Global::FramesProcessed, Global::NumFrames);

    this->videoFrame.UpdateColorMap(Global::VideoFrame);
    ImGui::Image((void*)(intptr_t)this->videoFrame.GetTextureID(), ImVec2(640, 480));

    ImGui::Text(Global::CurrentVideoFile.c_str());

    ImGui::End();
}

}
