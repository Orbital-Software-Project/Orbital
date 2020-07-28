#include "Camera.h"

namespace Orb {

Camera::Camera(std::shared_ptr<Shader> shader)  {

    // Init view matrix
    this->view = glm::mat4(1.0f);

    std::vector<Vertex> cam_vertices = {
        Vertex({0.5f,  0.5f, 0.0f},  {1.0f, 1.0f, 1.0f}),
        Vertex({0.5f, -0.5f, 0.0f},  {1.0f, 1.0f, 1.0f}),
        Vertex({-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}),
        Vertex({-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f})
    };

    std::vector<unsigned int> indices = {
        0,
        1,
        3,
        1,
        2,
        3
    };


    this->mesh = std::make_shared<Mesh>(shader, std::make_shared<MeshData>(cam_vertices, indices));
}

Camera::~Camera() {

}


const glm::mat4 Camera::GetViewMat() {
    return this->view;
}

void Camera::SetViewMat(glm::mat4 view) {
    this->view = view;
}

void Camera::Draw() {

    // Update pos of the camera
    this->mesh->SetModel(this->view);
    this->mesh->DrawOnlyVertColors(true);
    this->mesh->Draw();
}

}


