#include "CameraController.h"


namespace Orb {

CameraController::CameraController(GLFWwindow *window, std::shared_ptr<Camera> camera)  {
    this->window = window;
    this->camera = camera;
}

CameraController::~CameraController() {

}

void CameraController::DoEvents() {

    static float cameraSpeed = 0.1f;


    int keys[] = {GLFW_KEY_W, GLFW_KEY_S, GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_SPACE, GLFW_KEY_LEFT_CONTROL};

    for(int key : keys) {
        int state = glfwGetKey(window, key);
        if(state == GLFW_REPEAT) {
            switch(key) {
                case GLFW_KEY_W:
                this->camera->Position  += cameraSpeed * this->camera->CameraFront;
                    break;
                case GLFW_KEY_S:
                this->camera->Position -= cameraSpeed * this->camera->CameraFront;
                    break;
                case GLFW_KEY_A:
                this->camera->Position -= glm::normalize(glm::cross(this->camera->CameraFront, this->camera->CameraUp)) * cameraSpeed;
                    break;
                case GLFW_KEY_D:
                this->camera->Position += glm::normalize(glm::cross(this->camera->CameraFront, this->camera->CameraUp)) * cameraSpeed;
                    break;
                case GLFW_KEY_SPACE:
                this->camera->Position -= glm::vec3(0.0f, 0.1f, 0.0f);
                    break;
                case GLFW_KEY_LEFT_CONTROL:
                this->camera->Position += glm::vec3(0.0f, 0.1f, 0.0f);
                    break;
            }
        }
    }

    //this->camera->Matrix =
            //glm::lookAt(
            //            this->camera->Position,
            //            this->camera->Position + this->camera->CameraFront,
            //            this->camera->CameraUp
            //            );


}





}


