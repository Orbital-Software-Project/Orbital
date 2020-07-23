#include "Camera.h"

namespace Orb {

Camera::Camera()  {

    // Init view matrix
    this->view = glm::mat4(1.0f);

     std::vector<float> vertices = {
         0.5f,  0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
        -0.5f, -0.5f, 0.0f,
        -0.5f,  0.5f, 0.0f
    };

    std::vector<unsigned int> indices = {
        0,
        1,
        3,
        1,
        2,
        3
    };

    this->mesh = std::make_shared<Mesh>(vertices, indices);
}

Camera::~Camera() {

}


const glm::mat4 Camera::GetViewMat() {
    return this->view;
}

void Camera::SetViewMat(glm::mat4 view) {
    this->view = view;
}

void Camera::Draw(std::shared_ptr<Shader> shader) {

    // Update pos of the camera
    this->mesh->SetModel(this->view);

    this->mesh->Draw(shader);
}

}


