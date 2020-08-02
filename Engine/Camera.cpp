#include "Camera.h"

namespace Orb {

Camera::Camera(std::shared_ptr<Shader> shader)  {
    // Init view matrix
    data.ModelViewMat = glm::mat4(1.0f);

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
    return data.ModelViewMat;
}

void Camera::SetViewMat(glm::mat4 view) {
    data.ModelViewMat = view;
}

void Camera::Draw() {
    if(!this->visible) {
        return;
    }

    // Update pos of the camera
    this->mesh->SetModel(data.ModelViewMat);
    this->mesh->DrawOnlyVertColors(true);
    this->mesh->Draw();

}

void Camera::ToggleHide() {
    this->visible = !this->visible;
}

}


