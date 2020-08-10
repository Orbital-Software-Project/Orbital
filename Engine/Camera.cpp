#include "Camera.h"
#include "PrimitiveFactory.h"

namespace Orb {

Camera::Camera()  {
    // Init view matrix
    data.ModelViewMat = glm::mat4(1.0f);

    this->mesh = PrimitiveFactory::Plane();
}

Camera::~Camera() {

}

void Camera::Draw(std::shared_ptr<Shader> shader) {
    if(!this->visible) {
        return;
    }

    // Update pos of the camera
    this->mesh->SetMatrix(data.ModelViewMat);
    this->mesh->DrawOnlyVertColors(true);
    this->mesh->Draw(shader);
}

void Camera::SetVisible(bool visible) {
    this->visible = visible;
}

bool Camera::IsVisible() {
    return this->visible;
}


void Camera::SetMatrix(glm::mat4 matrix) {
    this->mesh->SetMatrix(matrix);
}

glm::mat4 Camera::GetMatrix() {
    return this->mesh->GetMatrix();
};

}


