#include "Mesh.h"

#include <GL/glew.h>
#include <iostream>

namespace Orb {

Mesh::Mesh() {
    this->init();
}

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices) {
    this->init();
    this->UpdateColored(vertices, indices);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &this->ebo);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void Mesh::UpdateColored(std::vector<Vertex> vertices_colored, std::vector<unsigned int> indices) {

    // if indices is empty then generate them
    if(indices.size() <= 0) {
        for(int i = 0; i < vertices_colored.size(); i++) {
            indices.push_back(i);
        }
    }

    std::vector<float> vertices;
    for(Vertex vert : vertices_colored) {
        vertices.push_back(vert.Pos.x);
        vertices.push_back(vert.Pos.y);
        vertices.push_back(vert.Pos.z);
        vertices.push_back(vert.Col.r);
        vertices.push_back(vert.Col.g);
        vertices.push_back(vert.Col.b);
    }

    this->vertices = vertices;
    this->vertices_colors = vertices_colored;
    this->indices = indices;
}

void Mesh::Draw(std::shared_ptr<Shader> shader) {

    // Bind vertex array object
    glBindVertexArray(this->vao);
    // Bind vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    // Set Data to draw
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3* sizeof(float)));
    glEnableVertexAttribArray(1);

    // Bind element buffer for indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);

    shader->Use();
    shader->SetMat4("model", this->model);

    glPointSize(3.0f);

    // Draw mesh
    glDrawElements(this->polygonMode, indices.size(), GL_UNSIGNED_INT, 0);

}

void Mesh::SetPolygonMode(GLenum polygonMode) {
    this->polygonMode = polygonMode;
}

void Mesh::SetModel(glm::mat4 model) {
    this->model = model;
}

std::vector<Vertex> Mesh::GetVertices() {
    this->vertices_colors;
}

std::vector<unsigned int> Mesh::GetIndices() {
    this->indices;
}

void Mesh::init() {
    glGenBuffers(1, &this->ebo);
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
}



}
