#pragma once

#include "Shader.h"
#include "Mesh.h"
#include "SceneRenderer.h"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <GL/glew.h>
#include <imgui.h>
#include <memory>

#include <openvslam/publish/map_publisher.h>

namespace Orb {

class MapViewer {

public:
    MapViewer(std::shared_ptr<SceneRenderer> renderer);

    ~MapViewer();

    void OnRender();

    void ImportMesh(std::string file);

    void Export(std::string file);


private:
    void updatePointCloudMesh();
    void updateCameraPos();
    void updateKeyFrames();
    void initGridMesh();


private:
    //std::shared_ptr<Shader> shader;
    std::shared_ptr<Shader> meshShader;

    std::shared_ptr<Mesh> gridMesh;
    std::shared_ptr<Mesh> pointCloud;
    std::shared_ptr<Mesh> keyframes;
    std::shared_ptr<Camera> camera;

    std::shared_ptr<SceneRenderer> renderer = nullptr;
    glm::mat4 view = glm::mat4(1.0f);


    glm::vec3 cameraPos   = glm::vec3(0.0f, 0.0f,  3.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp    = glm::vec3(0.0f, 1.0f,  0.0f);

};

}
