#include "MapViewer.h"
#include "Global.h"
#include "Utils.hpp"
#include "MeshImporter.h"

#include "PrimitiveFactory.h"

#include <stdint.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <openvslam/data/landmark.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Orb {

// https://stackoverflow.com/questions/9742840/what-are-the-steps-necessary-to-render-my-scene-to-a-framebuffer-objectfbo-and
// https://stackoverflow.com/questions/46541334/opengl-render-to-a-texture
// https://learnopengl.com/Advanced-OpenGL/Framebuffers

MapViewer::MapViewer(std::shared_ptr<SceneRenderer> renderer, std::shared_ptr<Shader> shader) {

    this->renderer = renderer;

    // Shader model
    this->meshShader = shader;

    this->pointCloud = std::make_shared<Mesh>();
    this->pointCloud->SetPolygonMode(GL_POINTS);
    this->pointCloud->DrawOnlyVertColors(true);
    this->renderer->AddEntity(this->pointCloud);


    this->viewportCam = std::make_shared<Camera>();
    this->viewportCam->Visible = false;
    this->renderer->AddEntity(this->viewportCam);

    this->slamCam = std::make_shared<Camera>();
    this->renderer->AddEntity(this->slamCam);



    this->keyframes = std::make_shared<Mesh>();
    this->keyframes->DrawOnlyVertColors(true);
    this->keyframes->SetPolygonMode(GL_LINE_STRIP);
    this->renderer->AddEntity(this->keyframes);

    this->initGridMesh();

    this->initVideoPlane();
}

MapViewer::~MapViewer() {

}

void MapViewer::OnRender() {
    ImGui::Begin("Map viewer");

    this->drawToolbar();

    // Get content size of the imgui window
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();

    // get current size of the imgui window
    ImVec2 vSize = ImVec2(vMax.x - vMin.x, vMax.y - (vMin.y + 30));

    // when the window is to small the framebuffer throws errors
    if(vSize.x < 50 || vSize.y < 50) {
        vSize = ImVec2(800, 600);
        ImGui::SetWindowSize(vSize);
    }

    // Update points
    this->updatePointCloudMesh();


    // Update reconstructed camera
    this->updateCameraPos();

    // Background video in 3d viewport
    this->updateVideoPlane(vSize.x, vSize.y, 2.5);


    // Update camera trajectory with its keyframes
    this->updateKeyFrames();


    this->gridMesh->Visible = !this->viewVirtualCamera;
    this->slamCam->Visible =  !this->viewVirtualCamera;


    this->meshShader->SetMat4("view", this->viewportCam->Matrix);

    // tmp to test fixed aspect ratio
    this->meshShader->SetMat4("projection", glm::perspective(glm::radians(45.0f), vSize .x / vSize.y, 0.1f, 100.0f));


    // Childframe to prevent movement of the window and enable viewport rotation
    ImGui::BeginChild("DragPanel", vSize, false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    // Draw opengl texture (viewport) as imgui image
    ImGui::Image((void*)(intptr_t)this->renderer->RenderToTexture(this->meshShader, vSize.x, vSize.y), vSize);


    // ImGui Viewport navigation
    {
        if(ImGui::IsWindowFocused()) {

            static bool dragging = false;
            static ImVec2 p0 = ImVec2(0, 0);
            static ImVec2 p1 = ImVec2(0, 0);

            auto *io = &ImGui::GetIO();
            // Dragging logic for Viewport rotation
            if(io->MouseDown[0]) {
                static ImVec2 p1Old = ImVec2(0, 0);

                // First position from first click
                if(!dragging) {
                    // Get first mouse pos
                    p0 = io->MousePos;
                    p1 = io->MousePos;
                    p1Old = ImVec2(0, 0);
                    dragging = true;

                } else {
                    // Update second mouse position to calc offset from p0
                    p1 = io->MousePos;
                }

                // Only apply rotation when cursor has moved
                if(p1Old.x != p1.x || p1Old.y != p1.y) {

                    ImVec2 delta(p0.x - p1.x , p1.y - p0.y);

                    float sensitivity = 0.1f;

                    this->viewportCam->Yaw -= (delta.x * sensitivity);
                    this->viewportCam->Pitch += (delta.y * sensitivity);

                    if(this->viewportCam->Pitch > 89.0f) { this->viewportCam->Pitch = 89.0f; }
                    if(this->viewportCam->Pitch < -89.0f) { this->viewportCam->Pitch = -89.0f; }

                    glm::vec3 direction;
                    direction.x = std::cos(glm::radians(this->viewportCam->Yaw )) * cos(glm::radians(this->viewportCam->Pitch));
                    direction.y = std::sin(glm::radians(this->viewportCam->Pitch));
                    direction.z = std::sin(glm::radians(this->viewportCam->Yaw )) * cos(glm::radians(this->viewportCam->Pitch));

                    this->viewportCam->CameraFront = glm::normalize(direction);
                }
                p1Old = p1;
            } else {
                // Reset dragging
                dragging = false;

                p0 = ImVec2(0, 0);
                p1 = ImVec2(0, 0);
            }


            static float cameraSpeed = 0.1f;

            // Handle viewport navigation with keys
            if(ImGui::IsKeyPressed(GLFW_KEY_W)) {
                this->viewportCam->Position += cameraSpeed * this->viewportCam->CameraFront;

            } else if (ImGui::IsKeyPressed(GLFW_KEY_S)) {
                this->viewportCam->Position -= cameraSpeed * this->viewportCam->CameraFront;

            } else if (ImGui::IsKeyPressed(GLFW_KEY_A)) {
                this->viewportCam->Position -= glm::normalize(glm::cross(this->viewportCam->CameraFront, this->viewportCam->CameraUp)) * cameraSpeed;

            } else if (ImGui::IsKeyPressed(GLFW_KEY_D)) {
                this->viewportCam->Position += glm::normalize(glm::cross(this->viewportCam->CameraFront, this->viewportCam->CameraUp)) * cameraSpeed;

            } else if (ImGui::IsKeyPressed(GLFW_KEY_SPACE)) {
                this->viewportCam->Position -= glm::vec3(0.0f, 0.1f, 0.0f);

            } else if (ImGui::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
                this->viewportCam->Position += glm::vec3(0.0f, 0.1f, 0.0f);
            }


            if(!this->viewVirtualCamera) {
                this->viewportCam->Matrix = glm::lookAt(
                            this->viewportCam->Position,
                            this->viewportCam->Position + this->viewportCam->CameraFront,
                            this->viewportCam->CameraUp
                            );
            }

        }
    }

    ImGui::EndChild();

    ImGui::End();

    return;
}

void MapViewer::ImportMesh(std::string file) {
    MeshImporter exporter;
    for(std::shared_ptr<Mesh> meshdata : exporter.Import(file)) {
        meshdata->DrawOnlyVertColors(false);
        this->renderer->AddEntity(std::move(meshdata));
    }

}

void MapViewer::drawToolbar() {

    if(ImGui::Button("[-]")) {

        this->viewportCam->Matrix = glm::mat4(1.0f);
        this->viewportCam->Pitch = 0;
        this->viewportCam->Yaw = 0;
        this->viewportCam->Position    = glm::vec3(0.0f, 0.0f, 0.0f);
        this->viewportCam->CameraFront = glm::vec3(0.0f, 0.0f, 1.0f);
        this->viewportCam->CameraUp    = glm::vec3(0.0f, 1.0f, 0.0f);

    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Reset view");
    ImGui::SameLine();

    if(ImGui::Button("[x]")) {
        this->viewVirtualCamera = !this->viewVirtualCamera;
    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Toggle virtual camera");
    ImGui::SameLine();


    if(ImGui::Button("V")) {
        this->showVideoBackground = !this->showVideoBackground;
    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Toggle baground video");
    ImGui::SameLine();

    if(ImGui::Button("1080p")) {
        ImGui::SetWindowSize(ImVec2(1920, 1080));
    }
    if (ImGui::IsItemHovered())
        ImGui::SetTooltip("Set viewport to 1920x1080p");


    ImGui::Separator();


}

void MapViewer::updateCameraPos() {
    if(Global::GetInstance().MapPublisher.get() == nullptr) {
        return;
    }

    //http://ogldev.atspace.co.uk/index.html

    Eigen::Matrix4f camera_pos = Global::GetInstance().MapPublisher->get_current_cam_pose().inverse().transpose().cast<float>().eval();
    glm::mat4 converted = Utils::ToGLM_Mat4f(camera_pos);


    // With inverse i get the correct pos, and


    // Here is the fucking problem


    // Translation is correct
    // Rotation is fucked

    //converted = glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, -1.0f)) * converted ;

    // slamcam is correct
    this->slamCam->Matrix = converted;

    if(this->viewVirtualCamera) {
        //TODO: Inverse and scale
        this->viewportCam->Matrix = glm::inverse(this->slamCam->Matrix);

        //this->viewportCam->Matrix = glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 1.0f, -1.0f)) * this->viewportCam->Matrix;
        this->viewportCam->Matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f, 1.0f, -1.0f)) * this->viewportCam->Matrix;

        //std::cout << glm::to_string(this->viewportCam->Matrix) << std::endl;
    }

}

void MapViewer::updateKeyFrames() {

    if(Global::GetInstance().MapPublisher.get() == nullptr) {
        return;
    }

    std::vector<openvslam::data::keyframe*> keyFrames;
    Global::GetInstance().MapPublisher->get_keyframes(keyFrames);



    // Small performance optimization
    // Do not update the pointcloud for every frame
    static int keyFrameSize = 0;
    int currSize = keyFrames.size();
    if(currSize == keyFrameSize) {
        return;
    }
    keyFrameSize = currSize;



    std::vector<Vertex> vertices;
    const auto draw_edge = [&](const openvslam::Vec3_t& cam_center_1, const openvslam::Vec3_t& cam_center_2) {
        vertices.push_back(
                    Vertex({
                               cam_center_1.cast<float>().x(),
                               cam_center_1.cast<float>().y(),
                               cam_center_1.cast<float>().z()
                           }, {
                               0.0f, 1.0f, 1.0f
                           })
                    );

        vertices.push_back(
                    Vertex({
                               cam_center_2.cast<float>().x(),
                               cam_center_2.cast<float>().y(),
                               cam_center_2.cast<float>().z()
                           },{
                               0.0f, 1.0f, 1.0f
                           })
                    );
    };

    for (const auto keyFrame : keyFrames) {
        if (!keyFrame || keyFrame->will_be_erased()) {
            continue;
        }

        const openvslam::Vec3_t cam_center_1 = keyFrame->get_cam_center();

        // covisibility graph
        const auto covisibilities = keyFrame->graph_node_->get_covisibilities_over_weight(100);
        if (!covisibilities.empty()) {
            for (const auto covisibility : covisibilities) {
                if (!covisibility || covisibility->will_be_erased()) {
                    continue;
                }
                if (covisibility->id_ < keyFrame->id_) {
                    continue;
                }
                const openvslam::Vec3_t cam_center_2 = covisibility->get_cam_center();
                draw_edge(cam_center_1, cam_center_2);
            }
        }

        // spanning tree
        auto spanning_parent = keyFrame->graph_node_->get_spanning_parent();
        if (spanning_parent) {
            const openvslam::Vec3_t cam_center_2 = spanning_parent->get_cam_center();
            draw_edge(cam_center_1, cam_center_2);
        }

        // loop edges
        const auto loop_edges = keyFrame->graph_node_->get_loop_edges();
        for (const auto loop_edge : loop_edges) {
            if (!loop_edge) {
                continue;
            }
            if (loop_edge->id_ < keyFrame->id_) {
                continue;
            }
            const openvslam::Vec3_t cam_center_2 = loop_edge->get_cam_center();
            draw_edge(cam_center_1, cam_center_2);
        }
    }

    this->keyframes->UpdateColored(vertices);

}

void MapViewer::initGridMesh() {
    std::vector<float> gridVerticies;

    this->gridMesh = PrimitiveFactory::Grid();
    this->gridMesh->SetPolygonMode(GL_LINES);
    this->gridMesh->DrawOnlyVertColors(true);

    this->renderer->AddEntity(this->gridMesh);

}

void MapViewer::updatePointCloudMesh() {
    if(Global::GetInstance().MapPublisher.get() == nullptr) {
        return;
    }

    std::vector<openvslam::data::landmark*> allLandmarks;
    std::set<openvslam::data::landmark*> localLandmarks;
    Global::GetInstance().MapPublisher->get_landmarks(allLandmarks, localLandmarks);


    // Small performance optimization
    // Do not update the pointcloud for every frame
    static int allLandMarkSize = 0;
    int currSize = allLandmarks.size();
    if(currSize == allLandMarkSize) {
        return;
    }
    allLandMarkSize = currSize;



    if(allLandmarks.size() == 0) {
        return;
    }

    std::vector<Vertex> vertices;
    for(openvslam::data::landmark* currLandMark : allLandmarks) {
        if (!currLandMark || currLandMark->will_be_erased()) {
            continue;
        }

        if (localLandmarks.count(currLandMark)) {
            continue;
        }

        Eigen::Vector3d pos = currLandMark->get_pos_in_world();

        Vertex vertLandmarks({
                                 pos.cast<float>().eval().x(),
                                 pos.cast<float>().eval().y(),
                                 pos.cast<float>().eval().z()
                             }, {
                                 1.0f,
                                 1.0f,
                                 0.0f
                             });

        vertices.push_back(vertLandmarks);
    }

    for(openvslam::data::landmark* currLandMark : allLandmarks) {
        if (!currLandMark || currLandMark->will_be_erased()) {
            continue;
        }

        Eigen::Vector3d pos = currLandMark->get_pos_in_world();
        Vertex vertAllLandmarks({
                                    pos.cast<float>().eval().x(),
                                    pos.cast<float>().eval().y(),
                                    pos.cast<float>().eval().z()
                                }, {
                                    0.0f,
                                    0.0f,
                                    1.0f
                                });

        vertices.push_back(vertAllLandmarks);

    }

    this->pointCloud->UpdateColored(vertices);

}

void MapViewer::initVideoPlane() {

}

void MapViewer::updateVideoPlane(float width, float height, float depth) {
    // https://stackoverflow.com/questions/46578529/how-to-compute-the-size-of-the-rectangle-that-is-visible-to-the-camera-at-a-give

    // calc box
    float aspect    = width / height;

    width = 1;
    height = width / aspect;

    float t         = std::tan(glm::radians(45.0f) / 2);
    float newHeight = t * depth;

    // Prevent the image from flipping
    if(newHeight > 0) {
        newHeight *= -1;
    }

    float newWidth  = newHeight * aspect;

    if(this->videoPlane.get() == nullptr) {
        this->videoPlane = PrimitiveFactory::SizedPlane(newWidth, newHeight);
        this->videoTexture = std::make_shared<Texture>();
        this->videoPlane->Textures.push_back(this->videoTexture);
        this->renderer->AddEntity(this->videoPlane);
    }
    this->videoPlane->Visible = this->showVideoBackground;

    if(!this->videoPlane->Visible) {
        return;
    }

    glm::mat4 newMat = glm::translate(this->slamCam->Matrix, glm::vec3(0.0f, 0.0f, depth));
    videoPlane->Matrix = newMat;

    if(Global::GetInstance().FramePublisher.get() == nullptr) {
        return;
    }


    this->videoTexture->UpdateTexture(Global::GetInstance().FramePublisher->draw_frame(false));
}

}
