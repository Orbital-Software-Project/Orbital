#pragma once

#include "Shader.h"

#include <vector>
#include <memory>
#include <GL/glew.h>

namespace Orb {

class Mesh {

public:
    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);
    ~Mesh();

    void Update(std::vector<float> vertices, std::vector<unsigned int> indices);

    void Draw();

    void SetPolygonMode(GLenum polygonMode);
    void SetShader(std::shared_ptr<Shader> shader);


private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int vbo = 0, vao = 0, ebo = 0;
    GLenum polygonMode = GL_TRIANGLES;
    std::shared_ptr<Shader> shader;

};

}
