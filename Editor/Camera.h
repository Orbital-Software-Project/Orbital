#pragma once

#include <Eigen/Core>

namespace Orb {

class Camera {


public:
    Camera();
    ~Camera();

private:
    float fov = 90.0f;
    Eigen::Vector3f pos;
    Eigen::Vector3f rot;


};

}
