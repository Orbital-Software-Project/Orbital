#include "Mesh.h"

#include <GL/glew.h>
#include <iostream>

namespace Orb {

Mesh::Mesh(std::shared_ptr<Shader> shader) {
    this->shader = shader;
    this->meshData = std::make_shared<MeshData>();

    this->init();
}

Mesh::Mesh(std::shared_ptr<Shader> shader, std::shared_ptr<MeshData> meshData) {
    this->shader = shader;
    this->init();
    this->UpdateColored(meshData);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &this->ebo);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void Mesh::UpdateColored(std::shared_ptr<MeshData> meshData) {

    this->meshData = meshData;

    // Bind vertex array object
    glBindVertexArray(this->vao);

    // Bind vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    // Set Data to draw
    glBufferData(GL_ARRAY_BUFFER, this->meshData->Vertices.size() * sizeof(Vertex), this->meshData->Vertices.data(), GL_STATIC_DRAW);


    // vertex position
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->meshData->Indices.size() * sizeof(float), this->meshData->Indices.data(), GL_STATIC_DRAW);

}

void Mesh::Draw() {
    glBindVertexArray(this->vao);

    glPointSize(3.0f);

    this->shader->SetBool("OnlyVertColor", this->drawOnlyVertColors);
    this->shader->Use();
    this->shader->SetMat4("model", this->model);

    // Draw mesh
    glDrawElements(this->polygonMode, this->meshData->Indices.size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
}

void Mesh::DrawOnlyVertColors(bool option) {
    this->drawOnlyVertColors = option;
}

void Mesh::SetPolygonMode(GLenum polygonMode) {
    this->polygonMode = polygonMode;
}

void Mesh::SetModel(glm::mat4 model) {
    this->model = model;
}

std::vector<Vertex> Mesh::GetVertices() {
    return this->meshData->Vertices;
}

std::vector<unsigned int> Mesh::GetIndices() {
    return this->meshData->Indices;
}

void Mesh::init() {
    glGenBuffers(1, &this->ebo);
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
}



}
