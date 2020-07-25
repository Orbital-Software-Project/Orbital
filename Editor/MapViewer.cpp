#include "MapViewer.h"
#include "Global.h"
#include "Utils.hpp"

#include <stdint.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <openvslam/data/landmark.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>


// TODO:
// - Draw camera mesh [done]
// - Draw camera trajectory [done]
// - Improve viewport navigation
// - Draw local landmarks -> needs vertex colors
// - Filter view (show grid, landmarks, keyframes etc.)
// - Draw keyframes
// - Export via assimp
// - Fix crash when cancel and make restart of slam possible
// -! optional docking
// -! Windows support


// Design:
// (UI)MapViewer -> SceneRenderer -> Mesh
//                               ^-> Shader
//                               ^-> Camera
// SceneRenderer
// This classes contains all the entities
// Active camera is selectet to render from the cameras view
// Only one shader can be the active shader
//

namespace Orb {

// https://stackoverflow.com/questions/9742840/what-are-the-steps-necessary-to-render-my-scene-to-a-framebuffer-objectfbo-and
// https://stackoverflow.com/questions/46541334/opengl-render-to-a-texture
// https://learnopengl.com/Advanced-OpenGL/Framebuffers

MapViewer::MapViewer() {

    this->shader = std::make_shared<Shader>("Shaders/MapViewer.vs", "Shaders/MapViewer.fs");

    static std::vector<float> vertices;
    static std::vector<unsigned int> indices;

    this->pointCloud = std::make_shared<Mesh>(vertices, indices);
    this->pointCloud->SetPolygonMode(GL_POINTS);

    this->renderer = std::make_unique<SceneRenderer>();
    this->renderer->AddMesh(this->pointCloud);

    this->camera = std::make_shared<Camera>();
    this->renderer->AddCamera(this->camera);

    this->keyframes = std::make_shared<Mesh>();
    this->keyframes->SetPolygonMode(GL_LINE_STRIP);
    this->renderer->AddMesh(this->keyframes);


    this->initGridMesh();
}

MapViewer::~MapViewer() {}

void MapViewer::OnRender() {
    ImGui::Begin("Map viewer");

    // Get content size of the imgui window
    ImVec2 vMin = ImGui::GetWindowContentRegionMin();
    ImVec2 vMax = ImGui::GetWindowContentRegionMax();

    // get current size of the imgui window
    ImVec2 vSize = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);

    // Update points
    this->updatePointCloudMesh();

    // Update reconstructed camera
    this->updateCameraPos();

    // Update camera trajectory with its keyframes
    this->updateKeyFrames();

    // Set fragment point colors to red
    //this->shader->SetVec4("color", glm::vec4(0.0f, 0.0f, 255.0f, 1.0f));


    this->shader->SetMat4("view", this->view);
    this->shader->SetMat4("projection", glm::perspective(glm::radians(45.0f), vSize .x / vSize.y, 0.1f, 100.0f));

    // Childframe to prevent movement of the window and enable viewport rotation
    ImGui::BeginChild("DragPanel", vSize, false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    // Draw opengl texture (viewport) as imgui image
    ImGui::Image((void*)(intptr_t)this->renderer->Render(vSize.x, vSize.y, this->shader), vSize);


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

                    // Rotate camera up or down
                    this->view = glm::rotate(this->view, glm::radians(delta.y < 0 ? 1.0f : -1.0f), glm::vec3(1.0f, 0.0f, 0.0f));

                    // Rotate left or right
                    this->view = glm::rotate(this->view, glm::radians(delta.x < 0 ? 1.0f : -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));

                    p1Old = p1;

                }
            } else {
                // Reset dragging
                dragging = false;
            }

            // Handle viewport navigation with keys
            if(ImGui::IsKeyPressed(GLFW_KEY_W)) {
                // Go forward
                this->view = glm::translate(this->view, glm::vec3(0.0f, 0.0f, 1.0f));

            } else if (ImGui::IsKeyPressed(GLFW_KEY_S)) {
                // Go backward
                this->view = glm::translate(this->view, glm::vec3(0.0f, 0.0f, -1.0f));

            } else if (ImGui::IsKeyPressed(GLFW_KEY_D)) {
                // Go left
                this->view = glm::translate(this->view, glm::vec3(-1.0f, 0.0f, 0.0f));

            } else if (ImGui::IsKeyPressed(GLFW_KEY_A)) {
                // Go right
                this->view = glm::translate(this->view, glm::vec3(1.0f, 0.0f, 0.0f));
            }

        }
    }

    ImGui::EndChild();

    ImGui::End();

    return;
}

void MapViewer::updateCameraPos() {

    if(Global::MapPublisher.get() == nullptr) {
        return;
    }

    Eigen::Matrix4f camera_pos_wc = Global::MapPublisher->get_current_cam_pose().inverse().transpose().cast<float>().eval(); // inverse cw to wc;
    glm::mat4 converted = Utils::ToGLM_Mat4f(camera_pos_wc);

    this->camera->SetViewMat(converted);

}

void MapViewer::updateKeyFrames() {

    if(Global::MapPublisher.get() == nullptr) {
        return;
    }

    std::vector<openvslam::data::keyframe*> keyFrames;
    Global::MapPublisher->get_keyframes(keyFrames);

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    const auto draw_edge = [&](const openvslam::Vec3_t& cam_center_1, const openvslam::Vec3_t& cam_center_2) {
        //glVertex3fv(cam_center_1.cast<float>().eval().data());
        //glVertex3fv(cam_center_2.cast<float>().eval().data());

        vertices.push_back(cam_center_1.cast<float>().x());
        vertices.push_back(cam_center_1.cast<float>().y());
        vertices.push_back(cam_center_1.cast<float>().z());
        indices.push_back(indices.size());

        vertices.push_back(cam_center_2.cast<float>().x());
        vertices.push_back(cam_center_2.cast<float>().y());
        vertices.push_back(cam_center_2.cast<float>().z());
        indices.push_back(indices.size());
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

    this->keyframes->Update(vertices, indices);

}

void MapViewer::initGridMesh() {
    std::vector<float> gridVerticies;

    constexpr float interval_ratio = 0.1;
    constexpr float grid_min       = -100.0f * interval_ratio;
    constexpr float grid_max       = 100.0f  * interval_ratio;

    for (int x = -10; x <= 10; x += 1) {
        gridVerticies.push_back(x * 10.0f * interval_ratio);
        gridVerticies.push_back(0);
        gridVerticies.push_back(grid_min);

        gridVerticies.push_back(x * 10.0f * interval_ratio);
        gridVerticies.push_back(0);
        gridVerticies.push_back(grid_max);
    }

    for (int y = -10; y <= 10; y += 1) {
        gridVerticies.push_back(grid_min );
        gridVerticies.push_back(0);
        gridVerticies.push_back(y * 10.0f * interval_ratio);

        gridVerticies.push_back(grid_max);
        gridVerticies.push_back(0);
        gridVerticies.push_back(y * 10.0f * interval_ratio);
    }

    std::vector<unsigned int> indices;
    for(unsigned int i = 0; i < gridVerticies.size(); i++) {
        indices.push_back(i);
    }

    this->gridMesh = std::make_shared<Mesh>(gridVerticies, indices);
    this->gridMesh->SetPolygonMode(GL_LINES);

    this->renderer->AddMesh(this->gridMesh);
}

void MapViewer::updatePointCloudMesh() {
    if(Global::MapPublisher.get() == nullptr) {
        return;
    }

    std::vector<openvslam::data::landmark*> allLandmarks;
    std::set<openvslam::data::landmark*> localLandmarks;
    Global::MapPublisher->get_landmarks(allLandmarks, localLandmarks);

    if(allLandmarks.size() == 0) {
        return;
    }

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    for(openvslam::data::landmark* currLandMark : allLandmarks) {
        if (!currLandMark || currLandMark->will_be_erased()) {
            continue;
        }

        Eigen::Vector3d pos = currLandMark->get_pos_in_world();

        auto x = pos.cast<float>().eval().x();
        auto y = pos.cast<float>().eval().y();
        auto z = pos.cast<float>().eval().z();

        vertices.push_back(x);
        indices.push_back(indices.size());

        vertices.push_back(y);
        indices.push_back(indices.size());

        vertices.push_back(z);
        indices.push_back(indices.size());
    }


    this->pointCloud->Update(vertices, indices);
}

}
