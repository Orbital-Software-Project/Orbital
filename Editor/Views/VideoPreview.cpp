#include "VideoPreview.h"
#include "Editor/Global.h"


#include <cstdint>
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <imgui.h>

namespace Orb {

    VideoPreview::VideoPreview() {
        //this->videoFrame = std::make_unique<Texture>();
    }

    VideoPreview::~VideoPreview() {

    }

    void VideoPreview::OnRender() {

        ImGui::Begin("Video preview");

        if (Global::GetInstance().VideoFrame != nullptr) {

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
            int zoomSesitivity = 10;
            if (mw != 0) {
                zoom += (mw * zoomSesitivity);
            }



            float imgWidth  = Global::GetInstance().VideoFrame->GetColorMap().cols;
            float imgHeight = Global::GetInstance().VideoFrame->GetColorMap().rows;

            float aspect = imgWidth / imgHeight;


            ImGui::GetWindowDrawList()->AddImage(
                (void*)(intptr_t)Global::GetInstance().VideoFrame->GetTextureID(),
                ImVec2(
                    panPanelPos.x + imgViewPos.x,
                    panPanelPos.y + imgViewPos.y
                ),
                ImVec2(
                    panPanelPos.x + imgViewPos.x + imgWidth + (zoom * aspect),
                    panPanelPos.y + imgViewPos.y + imgHeight + (zoom)
                )
            );


            ImGui::EndChild();
        }

        ImGui::End();
    }

}
