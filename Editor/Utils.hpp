#pragma once


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <GL/glew.h>
#include <iostream>

#include <Eigen/Core>
#include <glm/glm.hpp>

namespace Orb {

class Utils {

public:
    static glm::mat4 ToGLM_Mat4f(Eigen::Matrix4f mat4) {


        glm::mat4 res = glm::mat4(1.0f);

        res[0][0] = mat4(0, 0);
        res[0][1] = mat4(0, 1);
        res[0][2] = mat4(0, 2);
        res[0][3] = mat4(0, 3);
        res[1][0] = mat4(1, 0);
        res[1][1] = mat4(1, 1);
        res[1][2] = mat4(1, 2);
        res[1][3] = mat4(1, 3);
        res[2][0] = mat4(2, 0);
        res[2][1] = mat4(2, 1);
        res[2][2] = mat4(2, 2);
        res[2][3] = mat4(2, 3);
        res[3][0] = mat4(3, 0);
        res[3][1] = mat4(3, 1);
        res[3][2] = mat4(3, 2);
        res[3][3] = mat4(3, 3);

        return res;
    }


};

}
