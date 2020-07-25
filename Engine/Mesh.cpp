#include "Mesh.h"

#include <GL/glew.h>
#include <iostream>

namespace Orb {

Mesh::Mesh() {
    this->init();

    std::vector<float> vertices;
    std::vector<unsigned int> indices;

    this->Update(vertices, indices);
}

Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices)  {
    this->init();
    this->Update(vertices, indices);
}

Mesh::Mesh(std::vector<Vertex> vertices) {
    this->init();

    std::vector<unsigned int> indices;
    for(int i=0; i < vertices.size(); i++) {
        indices.push_back(i);
    }

    std::vector<float> vertices_colored;
    for(Vertex vert : vertices) {
        vertices_colored.push_back(vert.Position[0]);
        vertices_colored.push_back(vert.Position[1]);
        vertices_colored.push_back(vert.Position[2]);
        vertices_colored.push_back(vert.Color[0]);
        vertices_colored.push_back(vert.Color[1]);
        vertices_colored.push_back(vert.Color[2]);
    }

    this->vertices = vertices_colored;
    this->indices  = indices;


    //this->Update(vertices_colored, indices);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &this->ebo);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void Mesh::Update(std::vector<float> vertices, std::vector<unsigned int> indices) {

    std::vector<float> vertices_colored;
    for(int i = 0; i < vertices.size(); i++) {
        if(i % 3 == 0 && i != 0) {
            vertices_colored.push_back(1.0f);
            vertices_colored.push_back(0.0f);
            vertices_colored.push_back(0.0f);
            vertices_colored.push_back(vertices[i]);
        } else {
            vertices_colored.push_back(vertices[i]);
        }
    }

    vertices_colored.push_back(1.0f);
    vertices_colored.push_back(0.0f);
    vertices_colored.push_back(0.0f);

    this->vertices = vertices_colored;
    this->indices  = indices;
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


void Mesh::init() {
    glGenBuffers(1, &this->ebo);
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);
}



}
