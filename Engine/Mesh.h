#pragma once

#include "IEntity.h"

#include "Texture.h"
#include "Vertex.hpp"

#include <vector>
#include <memory>
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <iostream>



namespace Orb {

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

    void Draw(std::shared_ptr<Shader> shader);

    IEntity::EntityType GetEntityType() { return  IEntity::EntityType::Mesh; }


public:
    std::vector<Vertex> Vertices;
    std::vector<unsigned int> Indices;
    std::vector<std::shared_ptr<Texture>> Textures;

private:
    void init();

    void updateOpenGLBuffer();

private:
    unsigned int vbo = 0, vao = 0, ebo = 0;
    GLenum polygonMode = GL_TRIANGLES;
    bool drawOnlyVertColors = false;

    bool isInit = true;
    bool requireOpenGLBufferUpdate = false;


};

}
