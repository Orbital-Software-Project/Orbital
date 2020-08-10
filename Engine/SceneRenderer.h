#pragma once

#include "IEntity.h"

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

    GLuint RenderToTexture(std::shared_ptr<Shader> shader,int width, int height);

    void Render(std::shared_ptr<Shader> shader, int width, int height);

    void RemoveEntity(std::shared_ptr<IEntity> entity);

    void AddEntity(std::shared_ptr<IEntity> entity);

    std::vector<std::shared_ptr<IEntity>> GetEntities();

private:
    void setFBOSize(int widht, int height);

private:
    GLuint fbo = 0;
    GLuint rbo = 0;
    GLuint mapTexture = 0;
    GLuint vbo = 0, vao = 0, ebo = 0;
    int width = 800, height = 600;

    std::vector<std::shared_ptr<IEntity>> entities;

};

}
