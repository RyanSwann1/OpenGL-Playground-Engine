#include "ModelLoader.h"
#include "Model.h"
#include "Texture.h"
#include "Globals.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace
{
    const std::string MODELS_DIRECTORY = "models/";
}

void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes, std::vector<std::unique_ptr<Texture>>& loadedTextures,
const std::string& directory);
Mesh processMesh(aiMesh* mesh, const aiScene* scene, std::vector<std::unique_ptr<Texture>>& loadedTextures, const std::string& directory);
void loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, std::vector<std::unique_ptr<Texture>>& loadedTextures,
    const std::string& directory, std::vector<std::reference_wrapper<const Texture>>& textures);

bool ModelLoader::loadModel(const std::string& fileName, std::vector<Mesh>& meshes, std::vector<std::unique_ptr<Texture>>& loadedTextures)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(MODELS_DIRECTORY + fileName, 
        aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << "\n";
        return false;
    }

    std::string directory = (MODELS_DIRECTORY + fileName).substr(0, (MODELS_DIRECTORY + fileName).find_last_of('/'));
    processNode(scene->mRootNode, scene, meshes, loadedTextures, directory);

    return true;
}

void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes, std::vector<std::unique_ptr<Texture>>& loadedTextures, 
    const std::string& directory)
{
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.emplace_back(processMesh(mesh, scene, loadedTextures, directory));
    }

    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, meshes, loadedTextures, directory);
    }
}

Mesh processMesh(aiMesh* mesh, const aiScene* scene, std::vector<std::unique_ptr<Texture>>& loadedTextures, const std::string& directory)
{
    std::vector<Vertex> vertices;
    vertices.reserve(static_cast<size_t>(mesh->mNumVertices));
    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        glm::vec3 position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
        glm::vec3 normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
        if (mesh->mTextureCoords[0]) 
        {
            vertices.emplace_back(position, normal, glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y));
        }
        else
        {
            vertices.emplace_back(position, normal, glm::vec2(0.0f, 0.0f));
        }
    }

    std::vector<unsigned int> indices;
    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
        {
            indices.push_back(face.mIndices[j]);
        }
    }
    
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    std::vector<std::reference_wrapper<const Texture>> textures;
   
    assert(Globals::getTexture(loadedTextures, Globals::DEFAULT_BLACK));
    textures.push_back(*Globals::getTexture(loadedTextures, Globals::DEFAULT_BLACK));

    assert(Globals::getTexture(loadedTextures, Globals::DEFAULT_MATERIAL));
    textures.push_back(*Globals::getTexture(loadedTextures, Globals::DEFAULT_MATERIAL));

    loadMaterialTextures(material, aiTextureType_DIFFUSE, Globals::TEXTURE_DIFFUSE, loadedTextures, directory, textures);
    loadMaterialTextures(material, aiTextureType_SPECULAR, Globals::TEXTURE_SPECULAR, loadedTextures, directory, textures);
    loadMaterialTextures(material, aiTextureType_HEIGHT, Globals::TEXTURE_NORMAL, loadedTextures, directory, textures);
    
    return Mesh(std::move(vertices), std::move(indices), std::move(textures));
}

void loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, std::vector<std::unique_ptr<Texture>>& loadedTextures,
    const std::string& directory, std::vector<std::reference_wrapper<const Texture>>& textures)
{
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString filePath;
        mat->GetTexture(type, i, &filePath);
        bool skip = false;
        for (const auto& loadedTexture : loadedTextures)
        {
            if (loadedTexture->path == std::string(filePath.C_Str()))
            {
                textures.push_back(*loadedTexture.get());
                skip = true;
                break;
            }
        }

        if (!skip)
        { 
            std::unique_ptr<Texture> texture = Texture::create(filePath.C_Str(), directory, typeName);
            assert(texture);
            if (texture)
            {
                loadedTextures.push_back(std::move(texture));
                textures.push_back(*loadedTextures.back());
            }
        }
    }
}