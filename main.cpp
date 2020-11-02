#include "Globals.h"
#include "SFML/Graphics.hpp"
#include "glad.h"
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderHandler.h"
#include "Model.h"
#include "Camera.h"
#include "ModelManager.h"
#include "GameObject.h"
#include "Light.h"
#include "imgui/imgui.h"
#include "imgui_impl/imgui_wrapper.h"
#include <vector>
#include <iostream>
#include <sstream>
#include <iomanip>

std::vector<GameObject> loadGameObjects()
{
	std::vector<GameObject> gameObjects;

	gameObjects.emplace_back(ModelManager::getInstance().getModel(SPONZA_MODEL_NAME),
		Transform(glm::vec3(), glm::vec3(0.75f, 0.75f, 0.75f)));

	gameObjects.emplace_back(ModelManager::getInstance().getModel(STANFORD_BUNNY_MODEL_NAME),
		Transform({}, { 2000.0f, 2000.0f, 2000.0f }, { 0.0f, 90.0f, 0.0f }));
	
	gameObjects.emplace_back(ModelManager::getInstance().getModel(STANFORD_DRAGON_MODEL_NAME),
		Transform({ 600.0f, 0.0f, 0.0f }, { 20.0f, 20.0f, 20.0f }, { 0.0f, 90.0f, 0.0f }));
	
	gameObjects.emplace_back(ModelManager::getInstance().getModel(LUCY_STATUE_MODEL_NAME),
		Transform({ -600.0f, 0.0f, 0.0f }, { 2.0f, 2.0f, 2.0f }, { 0.0f, -90.0f, 0.0f }));		

	return gameObjects;
}

std::stringstream applyPrecision(int decimalPlaces, float value)
{
	std::stringstream stringStream;
	stringStream << std::fixed << std::setprecision(decimalPlaces) << value;
	
	return stringStream;
}

void displayOverlayGUI(const Camera& camera)
{
	bool static p_open = true;
	const float DISTANCE = 10.0f;
	static int corner = 0;
	ImGuiIO& io = ImGui::GetIO();
	io.MouseDrawCursor = false;
	if (corner != -1)
	{
		ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
		ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
		ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
	}
	ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
	if (ImGui::Begin("Example: Simple overlay", &p_open, (corner != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
	{
		ImGui::Text("Camera Position");
		ImGui::Text(std::string(std::string("x: ") + applyPrecision(1, camera.position.x).str()).c_str());
		ImGui::Text(std::string(std::string("y: ") + applyPrecision(1, camera.position.y).str()).c_str());
		ImGui::Text(std::string(std::string("z: ") + applyPrecision(1, camera.position.z).str()).c_str());
		ImGui::Separator();
		ImGui::Text("Camera Rotation");
		ImGui::Text(std::string(std::string("x: ") + applyPrecision(1, camera.rotation.x).str()).c_str());
		ImGui::Text(std::string(std::string("y: ") + applyPrecision(1, camera.rotation.y).str()).c_str());
		
		if (ImGui::BeginPopupContextWindow())
		{
			if (ImGui::MenuItem("Custom", NULL, corner == -1)) corner = -1;
			if (ImGui::MenuItem("Top-left", NULL, corner == 0)) corner = 0;
			if (ImGui::MenuItem("Top-right", NULL, corner == 1)) corner = 1;
			if (ImGui::MenuItem("Bottom-left", NULL, corner == 2)) corner = 2;
			if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
			if (p_open && ImGui::MenuItem("Close")) p_open = false;
			ImGui::EndPopup();
		}
	}
	ImGui::End();
}

int main()
{
	sf::ContextSettings settings;
	settings.depthBits = 24;
	settings.stencilBits = 8;
	settings.antialiasingLevel = 4;
	settings.majorVersion = 3;
	settings.minorVersion = 3;
	settings.attributeFlags = sf::ContextSettings::Core;
	//glm::uvec2 windowSize(1920, 1080);
	//sf::Window window(sf::VideoMode(windowSize.x, windowSize.y), "OpenGL Playground", sf::Style::Fullscreen, settings);
	glm::uvec2 windowSize(1280, 800);
	sf::Window window(sf::VideoMode(windowSize.x, windowSize.y), "OpenGL Playground", sf::Style::Default, settings);
	window.setFramerateLimit(60);
	window.setMouseCursorVisible(false);
	
	gladLoadGL();
	glViewport(0, 0, windowSize.x, windowSize.y);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	ImGui_SFML_OpenGL3::init(window);

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

	const std::vector<GameObject> gameObjects = loadGameObjects();
	sf::Clock deltaClock;
	sf::Clock gameClock;
	Camera camera;
	Light light({ 0.0f, 100.0f, 0.0f }, { 1.0f, 1.0f, 0.0f });

	std::cout << glGetError() << "\n";
	std::cout << glGetError() << "\n";
	std::cout << glGetError() << "\n";
	std::cout << glGetError() << "\n";

	while (window.isOpen())
	{
		float deltaTime = deltaClock.restart().asSeconds();
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

		float timeElasped = gameClock.getElapsedTime().asSeconds();
		light.position.x = glm::sin(timeElasped * 2.0f) * 300.0f;
		//light.position.y = sin(timeElasped * 2.0f) * 250.0f;

		ImGui_SFML_OpenGL3::startFrame();
		displayOverlayGUI(camera);

		glm::mat4 view = glm::lookAt(camera.position, camera.position + camera.front, camera.up);
		glm::mat4 projection = glm::perspective(glm::radians(camera.FOV),
			static_cast<float>(windowSize.x) / static_cast<float>(windowSize.y), camera.nearPlaneDistance, camera.farPlaneDistance);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		assert(shaderHandler);
		shaderHandler->switchToShader(eShaderType::Default);
		shaderHandler->setUniformMat4f(eShaderType::Default, "uProjection", projection);
		shaderHandler->setUniformMat4f(eShaderType::Default, "uView", view);
		
		shaderHandler->setUniformVec3(eShaderType::Default, "uLightPosition", light.position);
		shaderHandler->setUniformVec3(eShaderType::Default, "uLightColor", light.color);
		for (const auto& gameObject : gameObjects)
		{
			gameObject.render(*shaderHandler);
		}

#ifdef DEBUG
		shaderHandler->switchToShader(eShaderType::Debug);
		shaderHandler->setUniformMat4f(eShaderType::Debug, "uProjection", projection);
		shaderHandler->setUniformMat4f(eShaderType::Debug, "uView", view);
		
		light.render(*shaderHandler);
#endif // DEBUG

		ImGui_SFML_OpenGL3::endFrame();
		window.display();
	}

	return 0;
}