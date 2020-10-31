#pragma once

#include "Model.h"
#include <memory>
#include <array>

extern const std::string SPONZA_MODEL_NAME;
extern const std::string STANFORD_BUNNY_MODEL_NAME;
inline const size_t MAX_MODELS = 2;

class ModelManager : private NonCopyable, private NonMovable
{
public:
	static ModelManager& getInstance() 
	{
		static ModelManager instance;
		return instance;
	}

	const std::array<std::unique_ptr<Model>, MAX_MODELS>& getAllModels() const;
	const Model& getModel(const std::string& modelName) const;
	bool isAllModelsLoaded() const;

private:
	ModelManager();

	const std::array<std::unique_ptr<Model>, MAX_MODELS> m_models;
};