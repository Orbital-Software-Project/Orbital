#include "Mesh.h"

#include <GL/glew.h>
#include <iostream>

namespace Orb {

Mesh::Mesh(std::vector<float> vertices, std::vector<unsigned int> indices)  {
    glGenBuffers(1, &this->ebo);
    glGenVertexArrays(1, &this->vao);
    glGenBuffers(1, &this->vbo);

    this->Update(vertices, indices);
}

Mesh::~Mesh() {
    glDeleteBuffers(1, &this->ebo);
    glDeleteVertexArrays(1, &this->vao);
    glDeleteBuffers(1, &this->vbo);
}

void Mesh::Update(std::vector<float> vertices, std::vector<unsigned int> indices) {
    this->vertices = vertices;
    this->indices  = indices;
}

void Mesh::Draw() {

    // Bind vertex array object
    glBindVertexArray(this->vao);
    // Bind vertex buffer object
    glBindBuffer(GL_ARRAY_BUFFER, this->vbo);

    // Set Data to draw
    glBufferData(GL_ARRAY_BUFFER, this->vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Set vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Bind element buffer for indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->ebo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(float), indices.data(), GL_STATIC_DRAW);

    if(this->shader.get() == nullptr) {
        std::cerr << "No shader defined." << std::endl;
        exit(-1);
        return;
    }

    this->shader->Use();

    glPointSize(3.0f);

    // Draw mesh
    glDrawElements(this->polygonMode, indices.size(), GL_UNSIGNED_INT, 0);

}

void Mesh::SetPolygonMode(GLenum polygonMode) {
    this->polygonMode = polygonMode;
}

void Mesh::SetShader(std::shared_ptr<Shader> shader) {
    this->shader = shader;
}



}
