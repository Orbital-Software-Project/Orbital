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

const glm::mat4 Camera::GetViewMat() {
    return data.ModelViewMat;
}

void Camera::SetViewMat(glm::mat4 view) {
    data.ModelViewMat = view;
}

void Camera::Draw(std::shared_ptr<Shader> shader) {
    if(!this->visible) {
        return;
    }

    // Update pos of the camera
    this->mesh->SetModel(data.ModelViewMat);
    this->mesh->DrawOnlyVertColors(true);
    this->mesh->Draw(shader);

}

void Camera::ToggleHide() {
    this->visible = !this->visible;
}

}


