#pragma once

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

}
