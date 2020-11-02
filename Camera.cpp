#include "Camera.h"
#include "Globals.h"
#include "glm/gtc/matrix_transform.hpp"

namespace
{
	constexpr float MOVEMENT_SPEED = 100.0f;
	constexpr float SENSITIVITY = 4.0f;
	constexpr float NEAR_PLANE_DISTANCE = 0.1f;
	constexpr float FAR_PLANE_DISTANCE = 15000.0f;
	constexpr float FIELD_OF_VIEW = 50.0f;
	constexpr float MIN_VELOCITY = 0.2f;
	constexpr float VELOCITY_DROPOFF = 0.9f;

	constexpr glm::vec3 STARTING_POSITION = { 0.0f, 500.0f, 0.0 };
}

Camera::Camera()
	: FOV(FIELD_OF_VIEW),
	sensitivity(SENSITIVITY),
	nearPlaneDistance(NEAR_PLANE_DISTANCE),
	farPlaneDistance(FAR_PLANE_DISTANCE),
	front(),
	up({ 0.0f, 1.0f, 0.0f }),
	right(),
	rotation(),
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

	if (glm::abs(rotation.y) >= 360.0f)
	{
		rotation.y = 0.0f;
	}

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

	velocity *= VELOCITY_DROPOFF;
	setFront();
	right = glm::normalize(glm::cross(front, { 0.0f, 1.0f, 0.0f }));
	up = glm::normalize(glm::cross(right, front));
	if (glm::abs(velocity.x) <= MIN_VELOCITY)
	{
		velocity.x = 0.0f;
	}
	if (glm::abs(velocity.y) <= MIN_VELOCITY)
	{
		velocity.y = 0.0f;
	}
	if (glm::abs(velocity.z) <= MIN_VELOCITY)
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