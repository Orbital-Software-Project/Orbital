#pragma once

#include "Mesh.h"
#include <string>


#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Orb {

class MeshExporter {

public:
    MeshExporter(std::shared_ptr<MeshData> mesh);

    ~MeshExporter();

    void Export(std::string file);

    static std::vector<std::shared_ptr<MeshData>> Import(std::string file);

private:
    static void processNode(aiNode *node, const aiScene *scene, std::vector<MeshData> &outMeshes);
    static MeshData processMesh(aiMesh *mesh, const aiScene *scene);

private:
    std::string file = "";
    std::shared_ptr<MeshData> mesh;
};

}
