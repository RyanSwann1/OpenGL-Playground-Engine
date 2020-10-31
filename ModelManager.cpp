#include "ModelManager.h"
#include "ModelManager.h"
#include <iostream>

const std::string SPONZA_MODEL_NAME = "sponza.obj";
const std::string STANFORD_BUNNY_MODEL_NAME = "stanford-bunny.obj";
const std::string STANFORD_DRAGON_MODEL_NAME = "stanford-dragon.obj";
const std::string LUCY_STATUE_MODEL_NAME = "metallic-lucy-statue-stanford-scan.obj";

namespace 
{	
	void loadModel(const std::string& fileName, std::array<std::unique_ptr<Model>, MAX_MODELS>& models)
	{
		std::unique_ptr<Model> model = Model::create(fileName);
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

	std::array<std::unique_ptr<Model>, MAX_MODELS> loadModels()
	{
		std::array<std::unique_ptr<Model>, MAX_MODELS> models;

		loadModel(SPONZA_MODEL_NAME, models);
		loadModel(STANFORD_BUNNY_MODEL_NAME, models);
		loadModel(STANFORD_DRAGON_MODEL_NAME, models);
		loadModel(LUCY_STATUE_MODEL_NAME, models);

		return models;
	}
}

ModelManager::ModelManager()
	: m_models(loadModels())
{}

const std::array<std::unique_ptr<Model>, MAX_MODELS>& ModelManager::getAllModels() const
{
	return m_models;
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

bool ModelManager::isAllModelsLoaded() const
{
	for (const auto& model : m_models)
	{
		if (!model)
		{
			return false;
		}
	}

	return true;
}