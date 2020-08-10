#include "PrimitiveFactory.h"

namespace Orb {

PrimitiveFactory::PrimitiveFactory()  {

}

PrimitiveFactory::~PrimitiveFactory() {

}

std::shared_ptr<Mesh> PrimitiveFactory::Cube() {
    std::vector<Vertex> vertices = {
        Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
        Vertex({ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
        Vertex({ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
        Vertex({ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
        Vertex({-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
        Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),

        Vertex({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
        Vertex({ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
        Vertex({ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
        Vertex({ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
        Vertex({-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
        Vertex({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),

        Vertex({-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
        Vertex({-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
        Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
        Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
        Vertex({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
        Vertex({-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),

        Vertex({ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
        Vertex({ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
        Vertex({ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
        Vertex({ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
        Vertex({ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
        Vertex({ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),

        Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
        Vertex({ 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
        Vertex({ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
        Vertex({ 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
        Vertex({-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
        Vertex({-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),

        Vertex({-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
        Vertex({ 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
        Vertex({ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
        Vertex({ 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
        Vertex({-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f}),
        Vertex({-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
    };

    std::shared_ptr<Mesh> meshCube = std::make_shared<Mesh>(vertices);

    return meshCube;
}

std::shared_ptr<Mesh> PrimitiveFactory::Grid() {

    constexpr float interval_ratio = 0.1;
    constexpr float grid_min       = -100.0f * interval_ratio;
    constexpr float grid_max       = 100.0f  * interval_ratio;

    std::vector<Vertex> gridVertices_color;

    for (int x = -10; x <= 10; x += 1) {
        Vertex p1({
                          x * 10.0f * interval_ratio,
                          0.0f,
                          grid_min
                      }, {
                          0.0f,
                          1.0f,
                          0.0f
                      });

        gridVertices_color.push_back(p1);

        Vertex p2({
                           x * 10.0f * interval_ratio,
                           0.0f,
                           grid_max
                       }, {
                           0.0f,
                           1.0f,
                           0.0f
                       });


        gridVertices_color.push_back(p2);
    }

    for (int z = -10; z <= 10; z += 1) {

        Vertex p1({
                          grid_min,
                          0.0f,
                          z * 10.0f * interval_ratio
                      }, {
                          0.0f,
                          1.0f,
                          0.0f
                      });


        gridVertices_color.push_back(p1);

        Vertex p2({
                           grid_max,
                           0.0f,
                           z * 10.0f * interval_ratio
                       }, {
                           0.0f,
                           1.0f,
                           0.0f
                       });

        gridVertices_color.push_back(p2);

    }

    return  std::make_shared<Mesh>(gridVertices_color);
}

std::shared_ptr<Mesh> PrimitiveFactory::Plane(){

    std::vector<Vertex> cam_vertices = {
        Vertex({0.5f,  0.5f, 0.0f},  {1.0f, 1.0f, 1.0f}),
        Vertex({0.5f, -0.5f, 0.0f},  {1.0f, 1.0f, 1.0f}),
        Vertex({-0.5f, -0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}),
        Vertex({-0.5f,  0.5f, 0.0f}, {1.0f, 1.0f, 1.0f})
    };

    std::vector<unsigned int> indices = {
        0,
        1,
        3,
        1,
        2,
        3
    };


    return std::make_shared<Mesh>(cam_vertices, indices);
}


std::shared_ptr<Mesh> PrimitiveFactory::SizedPlane(float width, float height) {

    std::vector<Vertex> cam_vertices = {
        Vertex({ width,  height, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f}),
        Vertex({ width, -height, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}),
        Vertex({-width, -height, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f}),
        Vertex({-width,  height, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f})
    };

    std::vector<unsigned int> indices = {
        0,
        1,
        3,
        1,
        2,
        3
    };

    return std::make_shared<Mesh>(cam_vertices, indices);
}

}


