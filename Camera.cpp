#include "Camera.h"
#include "Globals.h"
#include "glm/gtc/matrix_transform.hpp"

namespace
{
	constexpr float MOVEMENT_SPEED = 15.0f;
	constexpr float ZOOM_SPEED = 30.0f;

	constexpr int MAX_RAY_TO_GROUND_DISTANCE = 2500;
	constexpr float MINIMUM_HEIGHT = 5.0f;
	constexpr float SENSITIVITY = 4.0f;
	constexpr float NEAR_PLANE_DISTANCE = 0.1f;
	constexpr float FAR_PLANE_DISTANCE = 1750.0f;
	constexpr float FIELD_OF_VIEW = 50.0f;
	constexpr glm::vec3 STARTING_POSITION = { 0.0f, 72.0f, 43.0f };
	constexpr glm::vec3 STARTING_ROTATION = { -75.0f, 0.0f, 0.0f };
}

Camera::Camera()
	: FOV(FIELD_OF_VIEW),
	sensitivity(SENSITIVITY),
	nearPlaneDistance(NEAR_PLANE_DISTANCE),
	farPlaneDistance(FAR_PLANE_DISTANCE),
	front(),
	up({ 0.0f, 1.0f, 0.0f }),
	right(),
	rotation(STARTING_ROTATION),
	position(STARTING_POSITION),
	velocity()
{
	setFront();
	right = glm::normalize(glm::cross(front, up));
	up = glm::normalize(glm::cross(right, front));
}

glm::mat4 Camera::getView() const
{
	return glm::lookAt(position, position + front, up);
}

glm::mat4 Camera::getProjection(const sf::Window& window) const
{
	return glm::perspective(glm::radians(FOV),
		static_cast<float>(window.getSize().x) / static_cast<float>(window.getSize().y), nearPlaneDistance, farPlaneDistance);
}

void Camera::onMouseMove(float deltaTime, const sf::Window& window)
{
	rotation.x += (static_cast<int>(window.getSize().y / 2) - sf::Mouse::getPosition(window).y) * sensitivity * deltaTime;
	rotation.y += (sf::Mouse::getPosition(window).x - static_cast<int>(window.getSize().x / 2)) * sensitivity * deltaTime;

	sf::Mouse::setPosition(sf::Vector2i(window.getSize().x / 2, window.getSize().y / 2), window);

	setFront();
	right = glm::normalize(glm::cross(front, { 0.0f, 1.0f, 0.0f }));
	up = glm::normalize(glm::cross(right, front));	
}

void Camera::update(float deltaTime, const sf::Window& window)
{
	onMouseMove(deltaTime, window);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
	{
		velocity += front * MOVEMENT_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		velocity -= front * MOVEMENT_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		velocity += right * MOVEMENT_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
	{
		velocity -= right * MOVEMENT_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::E))
	{
		velocity += up * MOVEMENT_SPEED;
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		velocity -= up * MOVEMENT_SPEED;
	}

	position += velocity * deltaTime;

	velocity *= 0.9f;
	setFront();
	right = glm::normalize(glm::cross(front, { 0.0f, 1.0f, 0.0f }));
	up = glm::normalize(glm::cross(right, front));
	if (glm::abs(velocity.x) <= 0.2f)
	{
		velocity.x = 0.0f;
	}
	if (glm::abs(velocity.y) <= 0.2f)
	{
		velocity.y = 0.0f;
	}
	if (glm::abs(velocity.z) <= 0.2f)
	{
		velocity.z = 0.0f;
	}
}

void Camera::setFront()
{
	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (rotation.x > 89.0f)
		rotation.x = 89.0f;
	if (rotation.x < -89.0f)
		rotation.x = -89.0f;

	glm::vec3 v = {
		glm::cos(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x)),
		glm::sin(glm::radians(rotation.x)),
		glm::sin(glm::radians(rotation.y)) * glm::cos(glm::radians(rotation.x)) };
	front = glm::normalize(v);
}