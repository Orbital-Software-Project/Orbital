#include "Camera.h"

namespace Orb {

Camera::Camera()  {
    // Init view matrix
    this->view = Eigen::Matrix4f::Identity();

}

Camera::~Camera() { }


}
