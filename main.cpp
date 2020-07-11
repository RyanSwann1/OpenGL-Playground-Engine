#include "Globals.h"
#include "SFML/Graphics.hpp"
#include "glad.h"
#include "ShaderHandler.h"
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
	glm::uvec2 windowSize(640, 480);
	sf::Window window(sf::VideoMode(windowSize.x, windowSize.y), "OpenGL Playground", sf::Style::Default, settings);
	window.setFramerateLimit(60);
	gladLoadGL();
	glViewport(0, 0, windowSize.x, windowSize.y);

	std::unique_ptr<ShaderHandler> shaderHandler = ShaderHandler::create();
	assert(shaderHandler);
	if (!shaderHandler)
	{
		std::cout << "Failed to load Shader Handler\n";
		return -1;
	}

	shaderHandler->switchToShader(eShaderType::Default);

	unsigned int vaoID = Globals::INVALID_OPENGL_ID;
	glGenVertexArrays(1, &vaoID);
	glBindVertexArray(vaoID);

	std::vector<glm::vec2> positions;
	positions.insert(positions.end(), Globals::QUAD_COORDS.begin(), Globals::QUAD_COORDS.end());
	unsigned int positionsID = Globals::INVALID_OPENGL_ID;
	glGenBuffers(1, &positionsID);
	glBindBuffer(GL_ARRAY_BUFFER, positionsID);
	glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(glm::vec2), positions.data(), GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, sizeof(glm::vec2), (const void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	while (window.isOpen())
	{
		sf::Event currentSFMLEvent;
		while (window.pollEvent(currentSFMLEvent))
		{
			if (currentSFMLEvent.type == sf::Event::Closed)
			{
				window.close();
			}
		}

		glClear(GL_COLOR_BUFFER_BIT);
		glBindVertexArray(vaoID);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		window.display();
	}

	glDeleteVertexArrays(1, &vaoID);
	glDeleteBuffers(1, &positionsID);

	return 0;
}