#include "Mesh.h"

#include <GL/glew.h>
#include <iostream>

#include "Vertex.hpp"

namespace Orb {

Mesh::Mesh() {
    this->isInit = true;
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures) {
    this->isInit = true;
    
    this->UpdateColored(vertices, indices, textures);
}

Mesh::~Mesh() {
    if (!this->isInit) {
        glDeleteBuffers(1, &this->ebo);
        glDeleteVertexArrays(1, &this->vao);
        glDeleteBuffers(1, &this->vbo);
    }
}

void Mesh::UpdateColored(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<std::shared_ptr<Texture>> textures) {

    if(indices.size() <= 0) {
        for(int i = 0; i < vertices.size(); i++) {
            indices.push_back(indices.size());
        }
    }

    this->Vertices = vertices;
    this->Indices = indices;
    this->Textures = textures;

    this->requireOpenGLBufferUpdate = true;

}

void Mesh::DrawOnlyVertColors(bool option) {
    this->drawOnlyVertColors = option;
}

void Mesh::SetPolygonMode(GLenum polygonMode) {
    this->polygonMode = polygonMode;
}

std::vector<Vertex> Mesh::GetVertices() {
    return this->Vertices;
}

std::vector<unsigned int> Mesh::GetIndices() {
    return this->Indices;
}

void Mesh::Draw(std::shared_ptr<Shader> shader) {
    if(!this->Visible) {
        return;
    }

    if (this->isInit) {
        this->init();
    }

    if (this->requireOpenGLBufferUpdate) {
        this->updateOpenGLBuffer();
        this->requireOpenGLBufferUpdate = false;
    }

    glBindVertexArray(this->vao);

    // Vertex points size
    glPointSize(3.0f);

    shader->Use();
    shader->SetBool("OnlyVertColor", this->drawOnlyVertColors);
    shader->SetMat4("model", this->Matrix);

    for(std::shared_ptr<Texture> texture : this->Textures) {
        texture->Bind(shader);
    }

    // Draw mesh
    glDrawElements(this->polygonMode, this->Indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Mesh::init() {
    this->Name = "Mesh";

    this->isInit = false;

    glGenBuffers(1, &this->ebo);
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
}

void Mesh::updateOpenGLBuffer() {


    // Bind vertex array object
    glBindVertexArray(this->vao);

    // Bind vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    // Set Data to draw
    glBufferData(GL_ARRAY_BUFFER, this->Vertices.size() * sizeof(Vertex), this->Vertices.data(), GL_STATIC_DRAW);

    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), nullptr);

    // vertex color
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Col));

    // vertex normal
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Nor));

    // vertex texture coordinate
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, UV));

    // Bind element buffer for indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->Indices.size() * sizeof(float), this->Indices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);

}


}
