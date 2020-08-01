#include "MeshExporter.h"


#include <GL/glew.h>
#include <iostream>
#include <vector>

#include <random>
#include <opencv2/core.hpp>

namespace Orb {

MeshExporter::MeshExporter(std::shared_ptr<MeshData> mesh) {
    this->mesh = mesh;
}

MeshExporter::~MeshExporter() {}

void MeshExporter::Export(std::string file, std::shared_ptr<Mesh> pointCloud, std::vector<CameraData> cameras) {
    aiScene scene;

    scene.mRootNode = new aiNode();

    // Create material
    scene.mMaterials = new aiMaterial*[ 1 ];
    scene.mMaterials[ 0 ] = nullptr;
    scene.mNumMaterials = 1;
    scene.mMaterials[ 0 ] = new aiMaterial();

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
        pMesh->mVertices[ itr - vVertices.begin() ] = aiVector3D( v.x, v.y, v.z );
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

        // Assimp matrix is rowmajor whereas glm is column major
        cameraNode->mTransformation = ;

    }


    Assimp::Exporter *exporter = new Assimp::Exporter();
    exporter->Export(&scene, "obj", file.c_str());








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
        outMeshes.push_back(MeshExporter::processMesh(mesh, scene));
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


    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Texture> textures;

    // 1. diffuse maps
    std::vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    // 2. specular maps
    std::vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    // 3. normal maps
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal");
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

    // 4. height maps
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height");
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());


    return MeshData(vertices, indices, textures);
}

std::vector<Texture> MeshExporter::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
    std::vector<Texture> textures;
    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        // check if texture was loaded before and if so, continue to next iteration: skip loading a new texture

        if(true)
        {   // if texture hasn't been loaded already, load it
            Texture texture;

            textures.push_back(texture);
            //texture.UpdateColorMap()

            //texture.id = TextureFromFile(str.C_Str(), this->directory);
            //texture.type = typeName;
            //texture.path = str.C_Str();
            //textures.push_back(texture);
            //textures_loaded.push_back(texture);  // store it as texture loaded for entire model, to ensure we won't unnecesery load duplicate textures.
        }
    }
    return textures;
}



}
