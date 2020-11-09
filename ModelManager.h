#pragma once

#include "Model.h"
#include "Texture.h"
#include <memory>
#include <array>

extern const std::string SPONZA_MODEL_NAME;
extern const std::string STANFORD_BUNNY_MODEL_NAME;
extern const std::string STANFORD_DRAGON_MODEL_NAME;
extern const std::string LUCY_STATUE_MODEL_NAME;
inline constexpr size_t MAX_MODELS = 4;

class ModelManager : private NonCopyable, private NonMovable
{
public:
	static std::unique_ptr<ModelManager> create();

	const Model& getModel(const std::string& modelName) const;

private:
	ModelManager(std::array<std::unique_ptr<Model>, MAX_MODELS>&& models, 
		std::vector<std::unique_ptr<Texture>>&& loadedTextures);

	const std::array<std::unique_ptr<Model>, MAX_MODELS> m_models;
	const std::vector<std::unique_ptr<Texture>> m_loadedTextures;
};