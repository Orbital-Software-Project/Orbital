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
    MapViewer();

    ~MapViewer();

    void OnRender();

    void ImportMesh(std::string file);


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

    std::unique_ptr<SceneRenderer> renderer;
    glm::mat4 view = glm::mat4(1.0f);

};

}
