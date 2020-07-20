#pragma once


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <GL/glew.h>

#include <imgui.h>

#include "Shader.h"
#include "Mesh.h"
#include "SceneRenderer.h"

#include <memory>

namespace Orb {

class MapViewer {

public:
    MapViewer();
    ~MapViewer();

    void OnRender();

private:
    void updatePointCloudMesh();
    void initGridMesh();


private:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<Mesh> gridMesh;
    std::shared_ptr<Mesh> pointCloud;

    std::unique_ptr<SceneRenderer> renderer;

};

}
