#pragma once

#include "Shader.h"

#include <vector>
#include <memory>
#include <GL/glew.h>

namespace Orb {

class Mesh {

public:
    Mesh();

    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);

    ~Mesh();

    void Update(std::vector<float> vertices, std::vector<unsigned int> indices);

    void Draw(std::shared_ptr<Shader> shader);

    void SetPolygonMode(GLenum polygonMode);

    void SetModel(glm::mat4 model);


private:
    void init();

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int vbo = 0, vao = 0, ebo = 0;
    GLenum polygonMode = GL_TRIANGLES;
    glm::mat4 model = glm::mat4(1.0f);

};

}
