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
    MapViewer(std::shared_ptr<SceneRenderer> renderer, std::shared_ptr<Shader> shader);

    ~MapViewer();

    void OnRender();

    void ImportMesh(std::string file);

private:
    void drawToolbar();

    void initGridMesh();

    void handleViewportNav(ImVec2 vMin, ImVec2 vMax);

private:
    std::shared_ptr<Shader> meshShader = nullptr;
    std::shared_ptr<Mesh> gridMesh = nullptr;
    std::shared_ptr<Camera> viewportCam = nullptr;
    std::shared_ptr<SceneRenderer> renderer = nullptr;

    bool showVideoBackground = false;
    float backVideoDistance = 2.5f;

    int currCameraIdx = 0;

};

}
