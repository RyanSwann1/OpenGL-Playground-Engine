#include "Globals.h"
#include "SFML/Graphics.hpp"
#include "glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderHandler.h"
#include "MeshGenerator.h"
#include "Mesh.h"
#include "Camera.h"
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
	glm::uvec2 windowSize(1280, 720);
	sf::Window window(sf::VideoMode(windowSize.x, windowSize.y), "OpenGL Playground", sf::Style::Default, settings);
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

	glm::vec3 ambientLightColour = { 1.0f, 0.5f, 0.31f };
	float ambientStrength = 0.1f;

	shaderHandler->switchToShader(eShaderType::Default);
	shaderHandler->setUniformVec3(eShaderType::Default, "uAmbientColor", ambientLightColour);
	shaderHandler->setUniform1f(eShaderType::Default, "uAmbientStrength", ambientStrength);

	Camera camera;
	Mesh cube;
	Mesh cube2;

	MeshGenerator::generateCubeMesh({ 0, 0, 10.0f }, cube);
	MeshGenerator::generateCubeMesh({ 0, 0, 20.0f }, cube2);

	std::cout << glGetError() << "\n";
	std::cout << glGetError() << "\n";
	std::cout << glGetError() << "\n";
	std::cout << glGetError() << "\n";

	while (window.isOpen())
	{
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

		camera.update(window);

		glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
		glm::mat4 projection = glm::perspective(glm::radians(camera.FOV),
			static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), camera.nearPlaneDistance, camera.farPlaneDistance);

		shaderHandler->setUniformMat4f(eShaderType::Default, "uProjection", projection);
		shaderHandler->setUniformMat4f(eShaderType::Default, "uView", view);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		cube.render();
		cube2.render();

		window.display();
	}

	return 0;
}