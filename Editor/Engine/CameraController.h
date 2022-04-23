#pragma once

#include <memory>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "Camera.h"


namespace Orb {

class CameraController {

public:
    CameraController(GLFWwindow *window, std::shared_ptr<Camera> camera);

    ~CameraController();

    void DoEvents();

private:

    GLFWwindow *window = nullptr;
    std::shared_ptr<Camera> camera = nullptr;

};

}
