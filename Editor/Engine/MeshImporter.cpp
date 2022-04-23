#include "MeshImporter.h"

#include <GL/glew.h>
#include <iostream>
#include <vector>

#include <filesystem>
namespace Orb {

MeshImporter::MeshImporter() {}

MeshImporter::~MeshImporter() {}

std::vector<std::shared_ptr<Mesh>> MeshImporter::Import(std::string file) {

    this->currFile = file;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(file.c_str(),
                                             aiProcess_Triangulate |
                                             aiProcess_FlipUVs |
                                             aiProcess_JoinIdenticalVertices |
                                             aiProcess_ValidateDataStructure|
                                             aiProcess_FixInfacingNormals |
                                             aiProcess_FindDegenerates |
                                             aiProcess_FindInvalidData |
                                             aiProcess_GenNormals |
                                             aiProcess_CalcTangentSpace |
                                             aiProcess_PreTransformVertices
                                             );

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return std::vector<std::shared_ptr<Mesh>>();
    }

    std::vector<std::shared_ptr<Mesh>> outMeshes;
    MeshImporter::processNode(scene->mRootNode, scene, outMeshes);


    return outMeshes;
}

void MeshImporter::processNode(aiNode *node, const aiScene *scene, std::vector<std::shared_ptr<Mesh>> &outMeshes)
{

    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<std::shared_ptr<Texture>> textures;

    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];


        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            vertices.push_back(Vertex({
                                          mesh->mVertices[i].x,
                                          mesh->mVertices[i].y,
                                          mesh->mVertices[i].z
                                      }, {
                                          mesh->mColors[0] ? mesh->mColors[0][i].r : 1.0f,
                                          mesh->mColors[0] ? mesh->mColors[0][i].g : 1.0f,
                                          mesh->mColors[0] ? mesh->mColors[0][i].b : 1.0f
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
            for(unsigned int j = 0; j < face.mNumIndices; j++) {
                indices.push_back(face.mIndices[j]);
            }
        }

        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

        // 1. diffuse maps
        std::vector<std::shared_ptr<Texture>> diffuseMaps = this->loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

        // 2. specular maps
        std::vector<std::shared_ptr<Texture>> specularMaps = this->loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

        // 3. normal maps
        std::vector<std::shared_ptr<Texture>> normalMaps = this->loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
        textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

        // 4. height maps
        std::vector<std::shared_ptr<Texture>> heightMaps = this->loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
        textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

        std::shared_ptr<Mesh> orbMesh = std::make_shared<Mesh>(vertices, indices, textures);

        outMeshes.push_back(std::move(orbMesh));
    }

    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene, outMeshes);
    }



}

std::vector<std::shared_ptr<Texture>> MeshImporter::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{

    std::vector<std::shared_ptr<Texture>> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString fileName;
        auto res = aiGetMaterialTexture(mat, type, i, &fileName);

        // get full path of texture file
        std::string texturePath = this->resolveTextureFilePath(fileName.C_Str());

        if(res == aiReturn_SUCCESS && std::filesystem::exists(texturePath)) {

            // For the moment only load color data/diffuse map: normal maps etc. are not supported
            if(type == aiTextureType_DIFFUSE) {

                // Check if texture is already loaded
                bool exists = false;
                for(std::shared_ptr<Texture> tex : textures) {
                    std::string path = tex->FilePath;
                    if(std::strcmp(path.c_str(), texturePath.c_str()) == 0) {
                        exists = true;
                        break;
                    }
                }

                // if texture doesnt already exist in vector then add new texture
                if(!exists) {
                    std::shared_ptr<Texture> texture = std::make_shared<Texture>(texturePath);
                    textures.push_back(texture);
                }


            }

        }

    }
    return textures;
}

std::string MeshImporter::resolveTextureFilePath(std::string textureName) {
    // check if texture comes with file path
    if(std::filesystem::exists(textureName)) {
        return textureName;
    }

    // Get directory of asset file and check in that directory
    std::string dir = this->getDirectoryPathFromFilePath(this->currFile);
    std::string filePath = this->concatPath(dir, textureName);
    // Texture exists in that directory
    if(std::filesystem::exists(filePath)) {
        return filePath;
    }

    // Texture not found
    return "";

}

std::string MeshImporter::getDirectoryPathFromFilePath(std::string filepath) {



    char pathSeparator = '/';

#ifdef _WIN32
    pathSeparator = '\\';
#endif

    return filepath.substr(0, filepath.find_last_of(pathSeparator));
}

std::string MeshImporter::concatPath(std::string part1, std::string part2) {
    char pathSeparator = '/';

#ifdef _WIN32
    pathSeparator = '\\';
#endif

    return part1 + pathSeparator + part2;

}


}
