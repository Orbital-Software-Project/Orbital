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

    std::vector<std::shared_ptr<Mesh>> Import(std::string file);

private:
    void processNode(aiNode *node, const aiScene *scene, std::vector<std::shared_ptr<Mesh>> &outMeshes);

    std::vector<std::shared_ptr<Texture>> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);

    std::string getDirectoryPathFromFilePath(std::string filepath);

    std::string concatPath(std::string part1, std::string part2);

    std::string resolveTextureFilePath(std::string textureName);

    inline glm::mat4 aiMatrix4x4ToGlm(const aiMatrix4x4* from)
    {
        glm::mat4 to;

        to[0][0] = (GLfloat)from->a1;
        to[0][1] = (GLfloat)from->b1;
        to[0][2] = (GLfloat)from->c1;
        to[0][3] = (GLfloat)from->d1;
        to[1][0] = (GLfloat)from->a2;
        to[1][1] = (GLfloat)from->b2;
        to[1][2] = (GLfloat)from->c2;
        to[1][3] = (GLfloat)from->d2;
        to[2][0] = (GLfloat)from->a3;
        to[2][1] = (GLfloat)from->b3;
        to[2][2] = (GLfloat)from->c3;
        to[2][3] = (GLfloat)from->d3;
        to[3][0] = (GLfloat)from->a4;
        to[3][1] = (GLfloat)from->b4;
        to[3][2] = (GLfloat)from->c4;
        to[3][3] = (GLfloat)from->d4;

        return to;
    }

private:
    std::string currFile = "";
};

}
