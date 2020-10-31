#include "Globals.h"
#include "SFML/Graphics.hpp"
#include "glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderHandler.h"
#include "Model.h"
#include "Camera.h"
#include "TextureArray.h"
#include "ModelManager.h"
#include "Entity.h"
#include <vector>
#include <iostream>

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;
	glm::uvec2 windowSize(1920, 1080);
	sf::Window window(sf::VideoMode(windowSize.x, windowSize.y), "OpenGL Playground", sf::Style::Fullscreen, settings);
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);

	gladLoadGL();
	glViewport(0, 0, windowSize.x, windowSize.y);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	std::unique_ptr<ShaderHandler> shaderHandler = ShaderHandler::create();
	assert(shaderHandler);
	if (!shaderHandler)
	{
		std::cout << "Failed to load Shader Handler\n";
		return -1;
	}

	if (!ModelManager::getInstance().isAllModelsLoaded())
	{
		std::cout << "Failed to load all models\n";
		return -1;
	}

	std::vector<Entity> entities;
	
	entities.emplace_back(ModelManager::getInstance().getModel(SPONZA_MODEL_NAME), glm::vec3(0.0f, 0.0f, 0.0f));
	entities.emplace_back(ModelManager::getInstance().getModel(STANFORD_BUNNY_MODEL_NAME), 
		glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(2500.0f, 2500.0f, 2500.0f), glm::vec3(0.0f, 90.0f, 0.0));
	entities.emplace_back(ModelManager::getInstance().getModel(STANFORD_DRAGON_MODEL_NAME),
		glm::vec3(600.0f, 0.0f, 0.0f), glm::vec3(25.0f, 25.0f, 25.0f), glm::vec3(0.0f, 90.0f, 0.0f));
	entities.emplace_back(ModelManager::getInstance().getModel(LUCY_STATUE_MODEL_NAME),
		glm::vec3(-600.0f, 0.0f, 0.0f), glm::vec3(2.5f, 2.5f, 2.5f), glm::vec3(0.0f, -90.0f, 0.0f));

	sf::Clock clock;
	Camera camera;
	
	shaderHandler->switchToShader(eShaderType::Default);
	std::cout << glGetError() << "\n";
	std::cout << glGetError() << "\n";
	std::cout << glGetError() << "\n";
	std::cout << glGetError() << "\n";

	while (window.isOpen())
	{
		float deltaTime = clock.restart().asSeconds();
		sf::Event currentSFMLEvent;
		while (window.pollEvent(currentSFMLEvent))
		{
			if (currentSFMLEvent.type == sf::Event::Closed)
			{
				window.close();
			}
			else if (currentSFMLEvent.type == sf::Event::KeyPressed &&
				currentSFMLEvent.key.code == sf::Keyboard::Escape)
			{
				window.close();
			}
		}

		camera.update(deltaTime, window);

		glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
		glm::mat4 projection = glm::perspective(glm::radians(camera.FOV),
			static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), camera.nearPlaneDistance, camera.farPlaneDistance);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		shaderHandler->switchToShader(eShaderType::Default);
		shaderHandler->setUniformMat4f(eShaderType::Default, "uProjection", projection);
		shaderHandler->setUniformMat4f(eShaderType::Default, "uView", view);
		
		for (const auto& entity : entities)
		{
			entity.render(*shaderHandler);
		}

		window.display();
	}

	return 0;
}