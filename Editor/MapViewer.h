#pragma once


#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>

#include <GL/glew.h>

#include <imgui.h>

#include "Shader.h"
#include "Mesh.h"

#include <memory>

namespace Orb {

class MapViewer {
public:
    MapViewer();
    ~MapViewer();

    void OnRender();

private:
    void drawViewportTexture(int width, int height);
    void drawLine3D(float x0, float y0, float z0, float x1, float y1, float z1);
    void drawGrid3D();
    void setFBOSize(int width, int height);
    void initGridMesh();

private:
    GLuint fbo = 0;
    GLuint mapTexture = 0;
    GLuint rbo = 0;

    GLuint vbo = 0, vao = 0, ebo = 0;
    std::unique_ptr<Shader> shader;
    std::unique_ptr<Mesh> testMesh;
    std::unique_ptr<Mesh> gridMesh;

};

}
