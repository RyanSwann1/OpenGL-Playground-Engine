#pragma once

#include "Model.h"
#include <memory>
#include <array>

extern const std::string SPONZA_MODEL_NAME;
extern const std::string STANFORD_BUNNY_MODEL_NAME;
extern const std::string STANFORD_DRAGON_MODEL_NAME;
extern const std::string LUCY_STATUE_MODEL_NAME;
inline const size_t MAX_MODELS = 4;

struct ModelManager : private NonCopyable, private NonMovable
{
	static std::unique_ptr<ModelManager> create();

	const Model& getModel(const std::string& modelName) const;

	const std::array<std::unique_ptr<Model>, MAX_MODELS> models;

private:
	ModelManager(std::array<std::unique_ptr<Model>, MAX_MODELS>&& models);
};