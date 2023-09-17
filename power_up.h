#pragma once
#ifndef POWER_UP_H
#define POWER_UP_H

#include <string>

#include <glm/glm.hpp>
#include <glad/glad.h>

#include "game_object.h"

const glm::vec2 SIZE(60.0f, 20.0f); // The size of a Power Up block
const glm::vec2 VELOCITY(0.0f, 150.0f); // Velocity a PowerUp block when spawned

/// PowerUp inherits its state and rendering functions from
/// GameObject but also holds extra information to state its
/// active duration and whenever it is activated or not.
/// The type of power up is stored as a string.
class PowerUp : public GameObject
{
public:
	//powerup state
	std::string Type;
	float		Duration;
	bool		Activated;
	//constructor
	PowerUp(std::string type, glm::vec3 color, float duration,
		glm::vec2 position, Texture2D texture)
		: GameObject(position, SIZE, texture, color, VELOCITY),
		Type(type), Duration(duration), Activated()
	{}
};
#endif 

