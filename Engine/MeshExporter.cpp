#include "MeshExporter.h"

#include <GL/glew.h>
#include <iostream>
#include <vector>

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

namespace Orb {

MeshExporter::MeshExporter(std::shared_ptr<MeshData> mesh) {
    this->mesh = mesh;
}

MeshExporter::~MeshExporter() {}

void MeshExporter::Export(std::string file) {
    std::cout << file << std::endl;
}

std::shared_ptr<MeshData> MeshExporter::Import(std::string file) {

    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string warn;
    std::string err;

    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, file.c_str());



    return std::make_shared<MeshData>();
}

}