#include "MapViewer.h"
#include "Global.h"


#include <stdint.h>
#include <iostream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <string>
#include <openvslam/data/landmark.h>
#include <glm/gtc/matrix_transform.hpp>




namespace Orb {

// https://stackoverflow.com/questions/9742840/what-are-the-steps-necessary-to-render-my-scene-to-a-framebuffer-objectfbo-and
// https://stackoverflow.com/questions/46541334/opengl-render-to-a-texture
// https://learnopengl.com/Advanced-OpenGL/Framebuffers

MapViewer::MapViewer() {


    this->shader = std::make_shared<Shader>("Shaders/MapViewer.vs", "Shaders/MapViewer.fs");


    static std::vector<float> vertices = {
        0.5f,  0.5f, 0.0f,   // top right
        0.5f, -0.5f, 0.0f,   // bottom right
        -0.5f, -0.5f, 0.0f,   // bottom left
        -0.5f,  0.5f, 0.0f    // top left
    };

    static std::vector<unsigned int> indices = {  // note that we start from 0!
                                                  0, 1, 3,   // first triangle
                                                  1, 2, 3    // second triangle
                                               };

    this->pointCloud = std::make_shared<Mesh>(vertices, indices);
    this->pointCloud->SetShader(this->shader);
    this->pointCloud->SetPolygonMode(GL_POINTS);

    this->renderer = std::make_unique<SceneRenderer>();
    this->renderer->AddMesh(this->pointCloud);

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

    // Set fragment point colors to red
    this->shader->SetVec4("color", glm::vec4(0.0f, 0.0f, 255.0f, 1.0f));

    this->shader->SetMat4("model", glm::mat4(1.0f));
    this->shader->SetMat4("view", this->view);


    this->shader->SetMat4("projection", glm::perspective(glm::radians(45.0f), vSize .x / vSize.y, 0.1f, 100.0f));



    ImGui::BeginChild("DragPanel", vSize, false, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);

    // Draw opengl texture (viewport) as imgui image
    ImGui::Image((void*)(intptr_t)this->renderer->Render(vSize.x, vSize.y ), vSize);

    // ImGui Viewport navigation
    {

        static bool dragging = false;
        static ImVec2 p0 = ImVec2(0, 0);
        static ImVec2 p1 = ImVec2(0, 0);

        auto *io = &ImGui::GetIO();
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

            //ImGui::Text("%f / %f  -  %f / %f", p0.x, p0.y, p1.x, p1.y);

            // TODO: Logic to rotate in viewport
            ImDrawList* drawList = ImGui::GetWindowDrawList();
            drawList->AddLine(p0, p1, 0xFF0000FF, 10.0f);


            // Only apply rotation when cursor has moved
            static ImVec2 p1Old = ImVec2(0, 0);
            if(p1Old.x != p1.x || p1Old.y != p1.y) {
                // Rotate camera up or down
                this->view = glm::rotate(this->view, glm::radians((p1.y - p0.y) / 10), glm::vec3(1.0f, 0.0f, 0.0f));
                // Rotate left or right
                this->view = glm::rotate(this->view, glm::radians((p1.x - p0.x) / 10), glm::vec3(0.0f, 1.0f, 0.0f));
                p1Old = p1;
            }



        } else {
            // Reset dragging
            dragging = false;
        }


        if(ImGui::IsKeyPressed(GLFW_KEY_W)) {
            // Go forward
            this->view = glm::translate(this->view, glm::vec3(0.0f, 0.0f, 1.0f));

        } else if (ImGui::IsKeyPressed(GLFW_KEY_S)) {
            this->view = glm::translate(this->view, glm::vec3(0.0f, 0.0f, -1.0f));

        }else if (ImGui::IsKeyPressed(GLFW_KEY_D)) {
            this->view = glm::translate(this->view, glm::vec3(-1.0f, 0.0f, 0.0f));

        }else if (ImGui::IsKeyPressed(GLFW_KEY_A)) {
            this->view = glm::translate(this->view, glm::vec3(1.0f, 0.0f, 0.0f));
        }

    }

    ImGui::EndChild();

    ImGui::End();

    return;
}

void MapViewer::initGridMesh() {
    std::vector<float> gridVerticies;

    constexpr float interval_ratio = 0.1;
    constexpr float grid_min       = -100.0f * interval_ratio;
    constexpr float grid_max       = 100.0f  * interval_ratio;

    for (int x = -10; x <= 10; x += 1) {
        gridVerticies.push_back(x * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_min);
        gridVerticies.push_back(0);

        gridVerticies.push_back(x * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_max);
        gridVerticies.push_back(0);
    }

    for (int y = -10; y <= 10; y += 1) {
        gridVerticies.push_back(y * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_min);
        gridVerticies.push_back(0);

        gridVerticies.push_back(y * 10.0f * interval_ratio);
        gridVerticies.push_back(grid_max);
        gridVerticies.push_back(0);
    }

    std::vector<unsigned int> indices;
    for(unsigned int i = 0; i < gridVerticies.size(); i++) {
        indices.push_back(i);
    }

    this->gridMesh = std::make_shared<Mesh>(gridVerticies, indices);
    this->gridMesh->SetPolygonMode(GL_LINES);
    this->gridMesh->SetShader(this->shader);

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
