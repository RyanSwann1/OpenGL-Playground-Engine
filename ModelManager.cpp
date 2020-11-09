#include "ModelManager.h"
#include "Globals.h"

#include <iostream>

const std::string SPONZA_MODEL_NAME = "sponza.obj";
const std::string STANFORD_BUNNY_MODEL_NAME = "stanford-bunny.obj";
const std::string STANFORD_DRAGON_MODEL_NAME = "stanford-dragon.obj";
const std::string LUCY_STATUE_MODEL_NAME = "metallic-lucy-statue-stanford-scan.obj";

namespace 
{	
	void loadModel(const std::string& fileName, std::array<std::unique_ptr<Model>, MAX_MODELS>& models, 
		std::vector<std::unique_ptr<Texture>>& loadedTextures)
	{
		std::unique_ptr<Model> model = Model::create(fileName, loadedTextures);
		assert(model);
		if (!model)
		{
			std::cout << "Failed to load " << fileName << "\n";
			return;
		}

		assert(std::find(models.cbegin(), models.cend(), nullptr) != models.cend() &&
			std::find_if(models.cbegin(), models.cend(), [&fileName](const auto& model)
		{
			if (model)
			{
				return model->name == fileName;
			}
		}) == models.cend());

		for (int i = 0; i < static_cast<int>(models.size()); ++i)
		{
			if (!models[i])
			{
				models[i] = std::move(model);
				break;
			}
		}
	}

	std::array<std::unique_ptr<Model>, MAX_MODELS> loadModels(std::vector<std::unique_ptr<Texture>>& loadedTextures)
	{
		std::array<std::unique_ptr<Model>, MAX_MODELS> models;
		
		loadModel(SPONZA_MODEL_NAME, models, loadedTextures);
		loadModel(STANFORD_BUNNY_MODEL_NAME, models, loadedTextures);
		loadModel(STANFORD_DRAGON_MODEL_NAME, models, loadedTextures);
		loadModel(LUCY_STATUE_MODEL_NAME, models, loadedTextures);

		return models;
	}
}

ModelManager::ModelManager(std::array<std::unique_ptr<Model>, MAX_MODELS>&& models, 
	std::vector<std::unique_ptr<Texture>>&& loadedTextures)
	: m_models(std::move(models)),
	m_loadedTextures(std::move(loadedTextures))	
{}

std::unique_ptr<ModelManager> ModelManager::create()
{
	std::vector<std::unique_ptr<Texture>> loadedTextures;

	std::unique_ptr<Texture> defaultBlackTexture = Texture::create("default_black.png", Globals::DEFAULT_BLACK);
	assert(defaultBlackTexture);
	if (!defaultBlackTexture)
	{
		std::cout << "Failed to load texture\n";
		return std::unique_ptr<ModelManager>();
	}

	std::unique_ptr<Texture> defaultMaterialTexture = Texture::create("default_material.png", Globals::DEFAULT_MATERIAL);
	assert(defaultMaterialTexture);
	if (!defaultMaterialTexture)
	{
		std::cout << "Failed to load texture\n";
		return std::unique_ptr<ModelManager>();
	}

	loadedTextures.push_back(std::move(defaultBlackTexture));
	loadedTextures.push_back(std::move(defaultMaterialTexture));
	std::array<std::unique_ptr<Model>, MAX_MODELS> models = loadModels(loadedTextures);
	for (const auto& model : models)
	{
		if (!model)
		{
			return std::unique_ptr<ModelManager>();
		}
	}

	return std::unique_ptr<ModelManager>(new ModelManager(std::move(models), std::move(loadedTextures)));
}

const Model& ModelManager::getModel(const std::string& modelName) const
{
	auto model = std::find_if(m_models.cbegin(), m_models.cend(), [&modelName](const auto& model)
	{
		return model->name == modelName;
	});
	assert(model != m_models.cend());

	return *(*model);
}