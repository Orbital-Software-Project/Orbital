#include "Camera.h"
#include "PrimitiveFactory.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Orb {

Camera::Camera()  {
    this->mesh = PrimitiveFactory::Plane();
    this->Name = "Camera";
}

Camera::~Camera() {

}

void Camera::Draw(std::shared_ptr<Shader> shader) {
    if(!this->Visible) {
        return;
    }

    // Update pos of the camera
    this->mesh->Matrix = this->Matrix;

    //this->mesh->Matrix = glm::inverse(this->Matrix);

    //this->mesh->Matrix = glm::translate(this->mesh->Matrix, glm::vec3(0.0f, 0.0f, -10.0f));


    this->mesh->DrawOnlyVertColors(true);
    this->mesh->Draw(shader);

}

}


