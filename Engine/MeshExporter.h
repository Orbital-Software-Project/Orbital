#pragma once

#include "Mesh.h"
#include "Camera.h"

#include <string>


#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Orb {

class MeshExporter {

public:
    MeshExporter(std::shared_ptr<MeshData> mesh);

    ~MeshExporter();

    static void Export(std::string file, std::shared_ptr<Mesh> pointCloud, std::vector<CameraData> cameras);

    static std::vector<std::shared_ptr<MeshData>> Import(std::string file);

private:
    static void processNode(aiNode *node, const aiScene *scene, std::vector<MeshData> &outMeshes);

    static MeshData processMesh(aiMesh *mesh, const aiScene *scene);

    static std::vector<Texture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

private:
    std::string file = "";
    std::shared_ptr<MeshData> mesh;
};

}
