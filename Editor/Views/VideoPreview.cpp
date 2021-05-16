#include "VideoPreview.h"
#include "Editor/Global.h"

#include "Engine/Video.h"

#include "Editor/Tasks/TaskWorker.h"
#include "Editor/Base/ScopeMutexLock.hpp"

#include <cstdint>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <imgui.h>




// TODO:
// - Add video sequencer
//  - List Video entites 
//  - Logic for playing videos while reconstruction (openvslam task)

namespace Orb {

    VideoPreview::VideoPreview() {

        this->Name = "Video preview";
    }

    VideoPreview::~VideoPreview() {

    }

    void VideoPreview::OnRender() {

        if (Global::GetInstance().SequencerFrame != nullptr) {

            static ImVec2 imgViewPos;
            float viewWidth = ImGui::GetWindowWidth();
            float viewHeight = ImGui::GetWindowHeight();


            ImGui::BeginChildFrame(1, ImVec2(viewWidth, viewHeight - 50));
            ImVec2 panPanelPos = ImGui::GetCursorScreenPos();



            if (ImGui::IsItemActive() && ImGui::IsMouseDragging(0)) {
                imgViewPos.x += ImGui::GetIO().MouseDelta.x;
                imgViewPos.y += ImGui::GetIO().MouseDelta.y;
            }

            float mw = ImGui::GetIO().MouseWheel;

            static int zoom = 0;
            int zoomSesitivity = 20;
            if (ImGui::IsWindowFocused() && mw != 0) {
                zoom += (mw * zoomSesitivity);
            }


            {
                float imgWidth = Global::GetInstance().SequencerFrame->GetColorMap().cols;
                float imgHeight = Global::GetInstance().SequencerFrame->GetColorMap().rows;
                float aspect = imgWidth / imgHeight;

                ImVec2 p_min = ImVec2(
                    panPanelPos.x + imgViewPos.x,
                    panPanelPos.y + imgViewPos.y
                );

                std::string sizeStr = std::to_string(static_cast<int>(imgWidth)) + "x" + std::to_string(static_cast<int>(imgHeight));

                ScopeMutexLock lock(Global::GetInstance().GlobalMutex);

                ImGui::GetWindowDrawList()->AddText(ImVec2(p_min.x, p_min.y - 15), IM_COL32(255, 255, 255, 255), sizeStr.c_str());
                ImGui::GetWindowDrawList()->AddImage(
                    (void*)(intptr_t)Global::GetInstance().SequencerFrame->GetTextureID(),
                    p_min,
                    ImVec2(
                        panPanelPos.x + imgViewPos.x + imgWidth + (zoom * aspect),
                        panPanelPos.y + imgViewPos.y + imgHeight + (zoom)
                    )
                );
            }
            ImGui::EndChild();

        }

    }

}
