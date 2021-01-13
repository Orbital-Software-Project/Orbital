#include "MapViewer.h"
#include "Editor/Global.h"
#include "Editor/Utils.hpp"
#include "MeshImporter.h"

#include "PrimitiveFactory.h"

#include "Editor/Tasks/MeshImportTask.h"
#include "Editor/Tasks/TaskWorker.h"

#include "Editor/Base/ScopeMutexLock.hpp"

#include <stdint.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <openvslam/data/landmark.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_inverse.hpp>


#include <map>

namespace Orb {

    // https://stackoverflow.com/questions/9742840/what-are-the-steps-necessary-to-render-my-scene-to-a-framebuffer-objectfbo-and
    // https://stackoverflow.com/questions/46541334/opengl-render-to-a-texture
    // https://learnopengl.com/Advanced-OpenGL/Framebuffers

    MapViewer::MapViewer(std::shared_ptr<SceneRenderer> renderer, std::shared_ptr<Shader> shader) {

        this->renderer = renderer;

        // Shader model
        this->meshShader = shader;

        this->viewportCam = std::make_shared<Camera>();
        this->viewportCam->VisibleInOutliner = false;
        this->viewportCam->Visible = false;
        this->viewportCam->SetName("Viewport camera");
        this->renderer->AddEntity(this->viewportCam);


        this->initGridMesh();
    }

    MapViewer::~MapViewer() {

    }

    void MapViewer::OnRender() {
        ScopeMutexLock lock(Global::GetInstance().GlobalMutex);


        ImGui::Begin("Map viewer");

        this->drawToolbar();




        // Get content size of the imgui window
        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

        // get current size of the imgui window
        ImVec2 vSize = ImVec2(vMax.x - vMin.x, vMax.y - (vMin.y + 30));



        this->gridMesh->Visible = (this->currCameraIdx == 0);

        // Set view matrix
        this->meshShader->SetMat4("view", this->viewportCam->Matrix);

        // use negative apsect ration to flip viewport because of openvslam
        // https://www.learnopengles.com/tag/aspect-ratio/
        this->meshShader->SetMat4("projection", glm::perspective(glm::radians(45.0f), -vSize.x / vSize.y, 0.1f, 100.0f));

        // Childframe to prevent movement of the window and enable viewport rotation
        ImGui::BeginChild("DragPanel", vSize, false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);



        // when the window is to small the framebuffer throws errors
        if (vSize.x < 50 || vSize.y < 50) {
            vSize = ImVec2(800, 600);
            ImGui::SetWindowSize(vSize);
        }
        else {
            this->handleViewportNav(vMin, vMax);
            // Draw opengl texture (viewport) as imgui image
            ImGui::Image((void*)(intptr_t)this->renderer->RenderToTexture(this->meshShader, vSize.x, vSize.y), vSize);
        }


        

        
        



        ImGui::EndChild();

        ImGui::End();

    }

    void MapViewer::drawToolbar() {

        if (ImGui::Button("[-]")) {
            this->viewportCam->Matrix = glm::mat4(1.0f);
            this->viewportCam->Pitch = 0;
            this->viewportCam->Yaw = 0;
            this->viewportCam->Position = glm::vec3(0.0f, 0.0f, 0.0f);
            this->viewportCam->CameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
            this->viewportCam->CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Reset view");
        }

        ImGui::SameLine();

        if (ImGui::Button("V")) {
            this->showVideoBackground = !this->showVideoBackground;
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Toggle baground video");
        }

        ImGui::SameLine();

        if (ImGui::Button("1080p")) {
            ImGui::SetWindowSize(ImVec2(1920, 1080));
        }
        if (ImGui::IsItemHovered()) {
            ImGui::SetTooltip("Set viewport to 1920x1080p");
        }


        if (this->showVideoBackground) {

            ImGui::SameLine();

            ImGui::SetNextItemWidth(255);
            ImGui::SliderFloat("", &this->backVideoDistance, 2.5f, 100.0f);
            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Background video distance");
            }

            ImGui::SameLine();

            if (ImGui::Button("Res")) {
                this->backVideoDistance = 2.5f;
            }

            if (ImGui::IsItemHovered()) {
                ImGui::SetTooltip("Reset distance");
            }


        }

        ImGui::SameLine();

        // Get cameras from scenerenderer

        // Get indexes of all camera entities
        std::vector<int> cameraIdxVec;
        std::vector<std::shared_ptr<IEntity>> cameraCollection = Global::GetInstance().Renderer->EntitiesByType(IEntity::EntityType::Camera);


        int idx = 0;
        for (auto ent : cameraCollection) {
            cameraIdxVec.push_back(idx++);
        }

        ImGui::SetNextItemWidth(100.0f);
        if (ImGui::BeginCombo("##combo", cameraCollection[this->currCameraIdx]->GetName().c_str()))
        {
            for (int n = 0; n < cameraCollection.size(); n++)
            {
                bool selected = (this->currCameraIdx == cameraIdxVec[n]);
                if (ImGui::Selectable(cameraCollection[cameraIdxVec[n]]->GetName().c_str(), selected)) {
                    this->currCameraIdx = cameraIdxVec[n];
                    this->selectedCam = std::dynamic_pointer_cast<Camera, IEntity>(cameraCollection[this->currCameraIdx]);
                    if (selected) {
                        ImGui::SetItemDefaultFocus();
                        
                    }
                }
            }
            ImGui::EndCombo();
        }

        ImGui::Separator();


    }

    void MapViewer::initGridMesh() {
        std::vector<float> gridVerticies;

        this->gridMesh = PrimitiveFactory::Grid();
        this->gridMesh->VisibleInOutliner = false;
        this->gridMesh->SetPolygonMode(GL_LINES);
        this->gridMesh->DrawOnlyVertColors(true);
        this->renderer->AddEntity(this->gridMesh);

    }

    void MapViewer::handleViewportNav(ImVec2 vMin, ImVec2 vMax) {

        if (this->currCameraIdx != 0) {
            this->viewportCam->Matrix = glm::inverse(this->selectedCam->Matrix);
            this->viewportCam->Matrix = glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, -1.0f)) * this->viewportCam->Matrix;

        } else {

            // ImGui Viewport navigation
            {

                if (ImGui::IsWindowFocused()) {

                    auto* io = &ImGui::GetIO();

                    // Dragging logic for Viewport rotation
                    if (io->MouseDown[0]) {

                        bool mouseInViewportArea = false;

                        // Check if cursor is in Viewport area

                        ImVec2 vMin = ImGui::GetWindowContentRegionMin();
                        ImVec2 vMax = ImGui::GetWindowContentRegionMax();

                        vMin.x += ImGui::GetWindowPos().x;
                        vMin.y += ImGui::GetWindowPos().y;
                        vMax.x += ImGui::GetWindowPos().x;
                        vMax.y += ImGui::GetWindowPos().y;

                        mouseInViewportArea =
                            io->MousePos.x >= vMin.x &&
                            io->MousePos.y >= vMin.y &&
                            io->MousePos.x <= vMax.x &&
                            io->MousePos.y <= vMax.y;


                        static ImVec2 p0 = ImVec2(0, 0);
                        static ImVec2 p1 = ImVec2(0, 0);

                        if (mouseInViewportArea) {


                            // Update second mouse position to calc offset from p0
                            p0 = p1;
                            p1 = ImVec2(io->MousePos.x - vMin.x, io->MousePos.y - vMin.y);


                            // Only apply rotation when cursor has moved
                            if (p0.x != p1.x || p0.y != p1.y) {

                                ImVec2 delta(p0.x - p1.x, p1.y - p0.y);

                                // filter delta so there are no camera rotation jumps in the viewport
                                if ((delta.x < 50 && delta.x > -50) && (delta.y < 50 && delta.y > -50)) {

                                    float sensitivity = 0.1f;

                                    this->viewportCam->Yaw += (delta.x * sensitivity);
                                    this->viewportCam->Pitch += (delta.y * sensitivity);

                                    if (this->viewportCam->Pitch > 89.0f) { this->viewportCam->Pitch = 89.0f; }
                                    if (this->viewportCam->Pitch < -89.0f) { this->viewportCam->Pitch = -89.0f; }

                                    glm::vec3 direction;
                                    direction.x = std::cos(glm::radians(this->viewportCam->Yaw)) * std::cos(glm::radians(this->viewportCam->Pitch));
                                    direction.y = std::sin(glm::radians(this->viewportCam->Pitch));
                                    direction.z = std::sin(glm::radians(this->viewportCam->Yaw)) * std::cos(glm::radians(this->viewportCam->Pitch));

                                    this->viewportCam->CameraFront = glm::normalize(direction);

                                }


                            }
                        }
                    }

                    static float cameraSpeed = 0.1f;

                    // Handle viewport navigation with keys
                    if (ImGui::IsKeyPressed(GLFW_KEY_W)) {
                        this->viewportCam->Position += cameraSpeed * this->viewportCam->CameraFront;
                    }
                    else if (ImGui::IsKeyPressed(GLFW_KEY_S)) {
                        this->viewportCam->Position -= cameraSpeed * this->viewportCam->CameraFront;
                    }
                    else if (ImGui::IsKeyPressed(GLFW_KEY_A)) {
                        this->viewportCam->Position += glm::normalize(glm::cross(this->viewportCam->CameraFront, this->viewportCam->CameraUp)) * cameraSpeed;
                    }
                    else if (ImGui::IsKeyPressed(GLFW_KEY_D)) {
                        this->viewportCam->Position -= glm::normalize(glm::cross(this->viewportCam->CameraFront, this->viewportCam->CameraUp)) * cameraSpeed;
                    }
                    else if (ImGui::IsKeyPressed(GLFW_KEY_SPACE)) {
                        this->viewportCam->Position -= glm::vec3(0.0f, 0.1f, 0.0f);
                    }
                    else if (ImGui::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
                        this->viewportCam->Position += glm::vec3(0.0f, 0.1f, 0.0f);
                    }

                    this->viewportCam->Matrix = glm::lookAt(
                        this->viewportCam->Position,
                        this->viewportCam->Position + this->viewportCam->CameraFront,
                        this->viewportCam->CameraUp
                    );

                }


            }

        }
    }

}
