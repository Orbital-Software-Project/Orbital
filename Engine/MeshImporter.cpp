#include "MeshImporter.h"

#include <GL/glew.h>
#include <iostream>
#include <vector>

#include <filesystem>
namespace Orb {

MeshImporter::MeshImporter() {}

MeshImporter::MeshImporter(std::shared_ptr<MeshData> mesh) {
    this->mesh = mesh;
}

MeshImporter::~MeshImporter() {}

void MeshImporter::Export(std::string file, std::shared_ptr<Mesh> pointCloud, std::vector<CameraData> cameras) {
    aiScene scene;

    scene.mRootNode = new aiNode();

    // Create material
    scene.mMaterials = new aiMaterial*[ 1 ];
    scene.mMaterials[0] = nullptr;
    scene.mNumMaterials = 1;
    scene.mMaterials[0] = new aiMaterial();

    // Create mesh
    scene.mMeshes = new aiMesh*[ 1 ];
    scene.mMeshes[0] = nullptr;
    scene.mNumMeshes = 1;
    scene.mMeshes[0] = new aiMesh();
    scene.mMeshes[0]->mMaterialIndex = 0;

    // Add mesh to root node
    scene.mRootNode->mMeshes = new unsigned int[ 1 ];
    scene.mRootNode->mMeshes[0] = 0;
    scene.mRootNode->mNumMeshes = 1;


    auto pMesh = scene.mMeshes[0];

    // Add vertices to mesh
    const auto& vVertices = pointCloud->GetVertices();
    pMesh->mVertices    = new aiVector3D[ vVertices.size() ];
    pMesh->mNumVertices = vVertices.size();
    for (auto itr = vVertices.begin(); itr != vVertices.end(); ++itr) {
        const auto& v = itr->Pos;
        pMesh->mVertices[itr - vVertices.begin()] = aiVector3D( v.x, v.y, v.z );
    }

    // Add cameras
    scene.mCameras = new aiCamera*[cameras.size()];
    scene.mNumCameras = cameras.size();
    for(int i = 0; i < cameras.size(); i++ ) {
        scene.mCameras[i] = nullptr;
        scene.mCameras[i] = new aiCamera();

        // Convert camera pos to position and lookat
        scene.mCameras[i]->mPosition = aiVector3D(cameras[i].ModelViewMat[3].x, cameras[i].ModelViewMat[3].y, cameras[i].ModelViewMat[3].z);
        scene.mCameras[i]->mName = std::to_string(i).c_str();

        //find the transformation matrix corresponding to the camera node
        aiNode* rootNode = scene.mRootNode;
        aiNode* cameraNode = rootNode->FindNode(scene.mCameras[i]->mName);

    }

    Assimp::Exporter *exporter = new Assimp::Exporter();
    exporter->Export(&scene, "obj", file.c_str());

}

std::vector<std::shared_ptr<MeshData>> MeshImporter::Import(std::string file) {

    this->currFile = file;

    std::vector<std::shared_ptr<MeshData>> meshes;

    Assimp::Importer importer;
    const aiScene *scene = importer.ReadFile(file.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

    if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return std::vector<std::shared_ptr<MeshData>>();
    }

    std::vector<std::shared_ptr<MeshData>> outMeshes;
    MeshImporter::processNode(scene->mRootNode, scene, outMeshes);

    for(std::shared_ptr<MeshData> mesh : outMeshes) {
        meshes.push_back(mesh);
    }

    return meshes;
}

void MeshImporter::processNode(aiNode *node, const aiScene *scene, std::vector<std::shared_ptr<MeshData>> &outMeshes)
{
    // process all the node's meshes (if any)
    for(unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
        outMeshes.push_back(this->loadMesh(mesh, scene));
    }

    // then do the same for each of its children
    for(unsigned int i = 0; i < node->mNumChildren; i++)
    {
        this->processNode(node->mChildren[i], scene, outMeshes);
    }

}

std::shared_ptr<MeshData> MeshImporter::loadMesh(aiMesh *mesh, const aiScene *scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;


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

    std::vector<std::shared_ptr<Texture>> textures;


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


    return std::make_shared<MeshData>(vertices, indices, textures);
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
                    std::string path = tex->GetFilePath();
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
