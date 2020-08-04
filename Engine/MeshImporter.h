#pragma once

#include "Mesh.h"
#include "Camera.h"

#include <string>


#include <assimp/Importer.hpp>
#include <assimp/Exporter.hpp>

#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Orb {

class MeshImporter {

public:
    MeshImporter();

    ~MeshImporter();

    void Export(std::string file, std::shared_ptr<Mesh> pointCloud, std::vector<CameraData> cameras);

    std::vector<std::shared_ptr<Mesh>> Import(std::string file);

private:
    void processNode(aiNode *node, const aiScene *scene, std::vector<std::shared_ptr<Mesh>> &outMeshes);

    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    std::string getDirectoryPathFromFilePath(std::string filepath);

    std::string concatPath(std::string part1, std::string part2);

    std::string resolveTextureFilePath(std::string textureName);

private:
    std::string currFile = "";
};

}
