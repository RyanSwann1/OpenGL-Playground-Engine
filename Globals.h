#pragma once

#include "glm/glm.hpp"
#include "Texture.h"
#include <functional>
#include <array>
#include <string>
#include <vector>
#include <memory>

namespace Globals
{
	extern const std::string TEXTURES_DIRECTORY;
	extern const std::string DEFAULT_BLACK;
	extern const std::string DEFAULT_MATERIAL;
	extern const std::string TEXTURE_DIFFUSE;
	extern const std::string TEXTURE_SPECULAR;
	extern const std::string TEXTURE_NORMAL;

	extern const std::string U_DIRECTIONAL_LIGHT;
	extern const std::string U_VIEW_POSITION;
	extern const std::string U_MODEL_MATRIX_NORMAL;
	extern const std::string U_PROJ_VIEW_MODEL;
	extern const std::string U_VIEW_MODEL;
	extern const std::string U_DIRECTIONAL_LIGHT_COLOR;
	extern const std::string U_DIRECTIONAL_LIGHT_INTENSITY;

	constexpr unsigned int INVALID_OPENGL_ID = 0;	

	inline const Texture* getTexture(const std::vector<std::reference_wrapper<const Texture>>& textures, const std::string& type)
	{
		auto texture = std::find_if(textures.cbegin(), textures.cend(), [&type](const auto& texture)
		{
			return texture.get().type == type;
		});

		if (texture != textures.cend())
		{
			return &(*texture).get();
		}

		return nullptr;
	}

	inline const Texture* getTexture(const std::vector<std::unique_ptr<Texture>>& loadedTextures, const std::string& type)
	{
		auto texture = std::find_if(loadedTextures.cbegin(), loadedTextures.cend(), [&type](const auto& texture)
		{
			return texture.get()->type == type;
		});
		
		if (texture != loadedTextures.cend())
		{
			return (*texture).get();
		}

		return nullptr;
	}
}