#pragma once

#include "Mesh.h"

#include <GL/glew.h>
#include <vector>
#include <memory>

namespace Orb {

class SceneRenderer {

public:
    SceneRenderer();

    ~SceneRenderer();

    GLuint Render(int width, int height);

    void AddMesh(std::shared_ptr<Mesh> mesh);

private:
    void setFBOSize(int widht, int height);

private:
    GLuint fbo = 0;
    GLuint rbo = 0;
    GLuint mapTexture = 0;
    GLuint vbo = 0, vao = 0, ebo = 0;
    int width = 800, height = 600;

    std::vector<std::shared_ptr<Mesh>> meshes;

};

}
