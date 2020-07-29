#pragma once

#include "Shader.h"

#include <vector>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>



namespace Orb {


struct Vertex {

    Vertex(glm::vec3 pos = {0.0f, 0.0f, 0.0f}, glm::vec3 col = {0.0f, 0.0f, 0.0f}, glm::vec3 nor = {0.0f, 0.0f, 0.0f}, glm::vec2 uv = {0.0f, 0.0f})
    {
        this->Pos = pos;
        this->Col = col;
        this->Nor = nor;
        this->UV  = uv;
    }

    glm::vec3 Pos;

    glm::vec3 Col;

    glm::vec3 Nor;

    glm::vec2 UV;

};

struct MeshData {
    MeshData( std::vector<Vertex> vertices = {}, std::vector<unsigned int> indices = {}) {

        // if indices is empty then generate them
        if(indices.size() <= 0) {
            for(int i = 0; i < vertices.size(); i++) {
                indices.push_back(i);
            }
        }

        this->Vertices = vertices;
        this->Indices  = indices;
    }

    std::vector<Vertex> Vertices;

    std::vector<unsigned int> Indices;
};

class Mesh {

public:
    Mesh(std::shared_ptr<Shader> shader);

    Mesh(std::shared_ptr<Shader> shader, std::shared_ptr<MeshData> meshData);

    ~Mesh();

    void UpdateColored(std::shared_ptr<MeshData> meshData);

    void Draw();

    void SetPolygonMode(GLenum polygonMode);

    void SetModel(glm::mat4 model);

    std::vector<Vertex> GetVertices();

    std::vector<unsigned int> GetIndices();

    void DrawOnlyVertColors(bool option);


private:
    void init();

private:
    std::shared_ptr<MeshData> meshData;
    unsigned int vbo = 0, vao = 0, ebo = 0;
    GLenum polygonMode = GL_TRIANGLES;
    glm::mat4 model = glm::mat4(1.0f);
    std::vector<float> uvCollection;
    std::shared_ptr<Shader> shader;
    bool drawOnlyVertColors = false;


};

}
