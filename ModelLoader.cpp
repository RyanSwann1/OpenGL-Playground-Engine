#include "ModelLoader.h"
#include "Model.h"
#include "Texture.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <iostream>
#include <SFML/Graphics.hpp>

namespace
{
    struct LoadedTexture
    {
        LoadedTexture(unsigned int ID, const std::string& type, const std::string& path)
            : ID(ID),
            type(type),
            path(path)
        {}

        int ID;
        std::string type;
        std::string path;
    };

    const std::string MODELS_DIRECTORY = "models/";
}

void processNode(aiNode* node, const aiScene* scene, std::vector<Mesh>& meshes, std::vector<std::unique_ptr<Texture>>& loadedTextures,
const std::string& directory);
Mesh processMesh(aiMesh* mesh, const aiScene* scene, std::vector<std::unique_ptr<Texture>>& loadedTextures, const std::string& directory);
void loadMaterialTextures(aiMaterial* mat, aiTextureType type, const std::string& typeName, std::vector<std::unique_ptr<Texture>>& loadedTextures,
    const std::string& directory, std::vector<std::reference_wrapper<const Texture>>& textures);
Material loadMaterial(aiMaterial* mat);

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
    loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse", loadedTextures, directory, textures);
    loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular", loadedTextures, directory, textures);
    loadMaterialTextures(material, aiTextureType_HEIGHT, "texture_normal", loadedTextures, directory, textures);

    return Mesh(std::move(vertices), std::move(indices), std::move(textures), loadMaterial(material));
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

Material loadMaterial(aiMaterial* mat) 
{
    Material material;
    aiColor3D color(0.0f, 0.0f, 0.0f);
    float shininess;
        
    mat->Get(AI_MATKEY_COLOR_DIFFUSE, color);
    material.diffuse = glm::vec3(color.r, color.g, color.b);

    mat->Get(AI_MATKEY_COLOR_AMBIENT, color);
    material.ambient = glm::vec3(color.r, color.g, color.b);

    mat->Get(AI_MATKEY_COLOR_SPECULAR, color);
    material.specular = glm::vec3(color.r, color.g, color.b);

    mat->Get(AI_MATKEY_SHININESS, shininess);
    material.shininess = shininess;

    return material;
}