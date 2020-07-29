#include "MeshExporter.h"

#include <GL/glew.h>
#include <iostream>
#include <vector>

#include <random>

namespace Orb {

MeshExporter::MeshExporter(std::shared_ptr<MeshData> mesh) {
    this->mesh = mesh;
}

MeshExporter::~MeshExporter() {}

void MeshExporter::Export(std::string file) {
    std::cout << file << std::endl;
}

std::vector<std::shared_ptr<MeshData>> MeshExporter::Import(std::string file) {

    std::vector<std::shared_ptr<MeshData>> meshes;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(file.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return std::vector<std::shared_ptr<MeshData>>();
    }

    std::vector<MeshData> outMeshes;
    MeshExporter::processNode(scene->mRootNode, scene, outMeshes);


    for(MeshData mesh : outMeshes) {
        meshes.push_back(std::make_shared<MeshData>(mesh));
    }

    return meshes;
}

void MeshExporter::processNode(aiNode *node, const aiScene *scene, std::vector<MeshData> &outMeshes)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        outMeshes.push_back(processMesh(mesh, scene));
    }

    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        MeshExporter::processNode(node->mChildren[i], scene, outMeshes);
    }

}

MeshData MeshExporter::processMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> dis(0, 1);

    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        vertices.push_back(Vertex({
                                      mesh->mVertices[i].x,
                                      mesh->mVertices[i].y,
                                      mesh->mVertices[i].z
                                  }, {
                                      mesh->mColors[0] ? mesh->mColors[0][i].r : dis(gen),
                                      mesh->mColors[0] ? mesh->mColors[0][i].g : dis(gen),
                                      mesh->mColors[0] ? mesh->mColors[0][i].b : dis(gen)
                                  }, {
                                      mesh->mNormals[i].x,
                                      mesh->mNormals[i].y,
                                      mesh->mNormals[i].z
                                  }, {
                                      mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i].x : 0.0f,
                                      mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][i].y : 0.0f
                                  }
                                  ));

    }

    // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
    for(unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        // retrieve all indices of the face and store them in the indices vector
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    if(mesh->mMaterialIndex >= 0)
    {

    }

    return MeshData(vertices, indices);
}

}
