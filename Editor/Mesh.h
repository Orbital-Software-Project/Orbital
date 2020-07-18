#pragma once

#include <vector>

namespace Orb {

class Mesh {

public:
    Mesh(std::vector<float> vertices, std::vector<unsigned int> indices);
    ~Mesh();

    void Draw();

private:
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
    unsigned int vbo = 0, vao = 0, ebo = 0;

};

}
