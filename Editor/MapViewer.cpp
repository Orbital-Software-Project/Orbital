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

namespace Orb {

// https://stackoverflow.com/questions/9742840/what-are-the-steps-necessary-to-render-my-scene-to-a-framebuffer-objectfbo-and
// https://stackoverflow.com/questions/46541334/opengl-render-to-a-texture
// https://learnopengl.com/Advanced-OpenGL/Framebuffers

MapViewer::MapViewer(std::shared_ptr<SceneRenderer> renderer, std::shared_ptr<Shader> shader) {

    this->renderer = renderer;

    // Shader pointcloud
    //this->shader = std::make_shared<Shader>("Shaders/MapViewer.vs", "Shaders/MapViewer.fs");

    // Shader model
    this->meshShader = shader;

    this->pointCloud = std::make_shared<Mesh>();
    this->pointCloud->SetPolygonMode(GL_POINTS);
    this->pointCloud->DrawOnlyVertColors(true);
    this->renderer->AddMesh(this->pointCloud);

    this->camera = std::make_shared<Camera>();
    this->renderer->AddCamera(this->camera);

    this->keyframes = std::make_shared<Mesh>();
    this->keyframes->DrawOnlyVertColors(true);
    this->keyframes->SetPolygonMode(GL_LINE_STRIP);
    this->renderer->AddMesh(this->keyframes);

    this->initGridMesh();
}

MapViewer::~MapViewer() {

}

void MapViewer::OnRender() {
    ImGui::Begin("Map viewer");

    // Get content size of the imgui window
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();

    // get current size of the imgui window
    ImVec2 vSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);

    // when the window is to small the framebuffer throws errors
    if(vSize.x < 50 || vSize.y < 50) {
        vSize = ImVec2(800, 600);
        ImGui::SetWindowSize(vSize);
    }

    // Update points
    this->updatePointCloudMesh();

    // Update reconstructed camera
    this->updateCameraPos();

    // Update camera trajectory with its keyframes
    this->updateKeyFrames();

    this->meshShader->SetMat4("view", this->view);
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
                // First position from first click
                if(!dragging) {
                    // Get first mouse pos
                    p0 = io->MousePos;
                    p1 = io->MousePos;
                    dragging = true;

                } else {
                    // Update second mouse position to calc offset from p0
                    p1 = io->MousePos;
                }

                // Only apply rotation when cursor has moved
                static ImVec2 p1Old = ImVec2(0, 0);
                if(p1Old.x != p1.x || p1Old.y != p1.y) {

                    ImVec2 delta(p0.x - p1.x , p1.y - p0.y);


                    static float sensitivity = 0.1f;

                    static float yaw = 0;
                    yaw -= (delta.x * sensitivity);

                    static float pitch = 0;
                    pitch += (delta.y * sensitivity);

                    if(pitch > 89.0f) { pitch = 89.0f; }
                    if(pitch < -89.0f) { pitch = -89.0f; }

                    glm::vec3 direction;
                    direction.x = std::cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                    direction.y = std::sin(glm::radians(pitch));
                    direction.z = std::sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                    this->cameraFront = glm::normalize(direction);


                    p1Old = p1;

                }
            } else {
                // Reset dragging
                dragging = false;
            }


            static float cameraSpeed = 0.5f;

            // Handle viewport navigation with keys
            if(ImGui::IsKeyPressed(GLFW_KEY_W)) {
                this->cameraPos += cameraSpeed * this->cameraFront;

            } else if (ImGui::IsKeyPressed(GLFW_KEY_S)) {
                this->cameraPos -= cameraSpeed * this->cameraFront;

            } else if (ImGui::IsKeyPressed(GLFW_KEY_A)) {
                this->cameraPos -= glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * cameraSpeed;

            } else if (ImGui::IsKeyPressed(GLFW_KEY_D)) {
                this->cameraPos += glm::normalize(glm::cross(this->cameraFront, this->cameraUp)) * cameraSpeed;

            } else if (ImGui::IsKeyPressed(GLFW_KEY_SPACE)) {
                this->cameraPos -= glm::vec3(0.0f, 0.1f, 0.0f);

            } else if (ImGui::IsKeyPressed(GLFW_KEY_LEFT_CONTROL)) {
                this->cameraPos += glm::vec3(0.0f, 0.1f, 0.0f);
            }

            this->view = glm::lookAt(this->cameraPos, this->cameraPos + this->cameraFront, this->cameraUp);

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
        this->renderer->AddMesh(std::move(meshdata));
    }

}

void MapViewer::Export(std::string file) {
    std::vector<CameraData> cameras;

    std::vector<openvslam::data::keyframe*> keyFrames;
    Global::GetInstance().MapPublisher->get_keyframes(keyFrames);

    for(openvslam::data::keyframe* kf : keyFrames) {
        CameraData cam;
        cam.ModelViewMat = Utils::ToGLM_Mat4f(kf->get_cam_pose_inv().transpose().cast<float>().eval());
        cameras.push_back(cam);
    }

    MeshImporter exporter;

    exporter.Export(file, this->pointCloud, cameras);
}

void MapViewer::updateCameraPos() {
    if(Global::GetInstance().MapPublisher.get() == nullptr) {
        return;
    }

    Eigen::Matrix4f camera_pos_wc = Global::GetInstance().MapPublisher->get_current_cam_pose().inverse().transpose().cast<float>().eval(); // inverse cw to wc;
    glm::mat4 converted = Utils::ToGLM_Mat4f(camera_pos_wc);

    this->camera->SetViewMat(converted);
}

void MapViewer::updateKeyFrames() {

    if(Global::GetInstance().MapPublisher.get() == nullptr) {
        return;
    }

    std::vector<openvslam::data::keyframe*> keyFrames;
    Global::GetInstance().MapPublisher->get_keyframes(keyFrames);

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
    this->renderer->AddMesh(this->gridMesh);
}

void MapViewer::updatePointCloudMesh() {
    if(Global::GetInstance().MapPublisher.get() == nullptr) {
        return;
    }

    std::vector<openvslam::data::landmark*> allLandmarks;
    std::set<openvslam::data::landmark*> localLandmarks;
    Global::GetInstance().MapPublisher->get_landmarks(allLandmarks, localLandmarks);

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
                                 0.0f  }
                             );

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
                                    1.0f  }
                                );

        vertices.push_back(vertAllLandmarks);
    }

    this->pointCloud->UpdateColored(vertices);
}


}
