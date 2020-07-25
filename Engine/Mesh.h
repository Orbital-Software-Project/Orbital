#pragma once

#include "Shader.h"

#include <vector>
#include <memory>
#include <GL/glew.h>

namespace Orb {


struct Vertex {
    Vertex() {
        this->Color[0] = 255.0f;
        this->Color[1] = 0.0f;
        this->Color[2] = 0.0f;

        this->Position[0] = 0.0f;
        this->Position[1] = 0.0f;
        this->Position[2] = 0.0f;
    }
    float Position[3];

    float Color[3];
};

class Mesh {

public:
    Mesh();

    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);

    Mesh(std::vector<Vertex> vertices);

    ~Mesh();

    void Update(std::vector<float> vertices, std::vector<unsigned int> indices);

    void Draw(std::shared_ptr<Shader> shader);

    void SetPolygonMode(GLenum polygonMode);

    void SetModel(glm::mat4 model);


private:
    void init();

private:
    std::vector<float> vertices;
    std::vector<Vertex> vertices_colors;

    std::vector<unsigned int> indices;

    unsigned int vbo = 0, vao = 0, ebo = 0;
    GLenum polygonMode = GL_TRIANGLES;
    glm::mat4 model = glm::mat4(1.0f);

};

}
