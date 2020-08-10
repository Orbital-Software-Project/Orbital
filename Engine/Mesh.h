#pragma once

#include "IEntity.h"

#include "Texture.h"

#include <vector>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>



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

class Mesh : public IEntity {

public:
    Mesh();

    Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices = {}, std::vector<std::shared_ptr<Texture>> textures = {});

    ~Mesh();

    void UpdateColored(std::vector<Vertex> vertices = {}, std::vector<unsigned int> indices = {}, std::vector<std::shared_ptr<Texture>> textures = {});


    void SetPolygonMode(GLenum polygonMode);

    std::vector<Vertex> GetVertices();

    std::vector<unsigned int> GetIndices();

    void DrawOnlyVertColors(bool option);


    virtual void SetMatrix(glm::mat4 model);

    virtual glm::mat4 GetMatrix();

    virtual void Draw(std::shared_ptr<Shader> shader);

    virtual void SetVisible(bool visible);

    virtual bool IsVisible();


public:
    std::vector<Vertex> Vertices;

    std::vector<unsigned int> Indices;

    std::vector<std::shared_ptr<Texture>> Textures;

private:
    void init();

private:
    unsigned int vbo = 0, vao = 0, ebo = 0;
    GLenum polygonMode = GL_TRIANGLES;
    glm::mat4 model = glm::mat4(1.0f);
    bool drawOnlyVertColors = false;
    bool visible = true;


};

}
