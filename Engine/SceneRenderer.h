#pragma once

#include "Mesh.h"
#include "Camera.h"

#include <GL/glew.h>
#include <vector>
#include <memory>

namespace Orb {

class SceneRenderer {

public:
    SceneRenderer();

    ~SceneRenderer();

    GLuint Render(int width, int height, std::shared_ptr<Shader> shader);

    void AddMesh(std::shared_ptr<Mesh> mesh);

    void AddCamera(std::shared_ptr<Camera> camera);

private:
    void setFBOSize(int widht, int height);

private:
    GLuint fbo = 0;
    GLuint rbo = 0;
    GLuint mapTexture = 0;
    GLuint vbo = 0, vao = 0, ebo = 0;
    int width = 800, height = 600;

    std::vector<std::shared_ptr<Mesh>> meshes;

    std::vector<std::shared_ptr<Camera>> cameras;

};

}
