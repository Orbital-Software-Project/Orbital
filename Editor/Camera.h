#pragma once

#include <Eigen/Core>

namespace Orb {

class Camera {

public:
    Camera();
    ~Camera();

private:
    Eigen::Matrix4f view;


};

}
