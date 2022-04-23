#pragma once

#include "Editor/Engine/Shader.h"
#include "Editor/Engine/Mesh.h"
#include "Editor/Engine/SceneRenderer.h"
#include "IView.hpp"

#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <GL/glew.h>
#include <imgui.h>
#include <memory>

#include <stella_vslam/publish/map_publisher.h>

namespace Orb {

class MapViewer : public IView {

public:
    MapViewer(std::shared_ptr<SceneRenderer> renderer, std::shared_ptr<Shader> shader);

    ~MapViewer();

    void OnRender();

private:
    void drawToolbar();

    void initGridMesh();

    void handleViewportNav(ImVec2 vMin, ImVec2 vMax);

private:
    std::shared_ptr<Shader> meshShader = nullptr;
    std::shared_ptr<Mesh> gridMesh = nullptr;
    std::shared_ptr<SceneRenderer> renderer = nullptr;


    std::shared_ptr<Camera> viewportCam = nullptr;

    std::shared_ptr<Camera> selectedCam = nullptr;



    bool showVideoBackground = false;
    float backVideoDistance = 2.5f;

    int currCameraIdx = 0;

};

}
