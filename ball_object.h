#pragma once
#ifndef BALLOBJECT_H
#define BALLOBJECT_H

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "texture.h"

/// BallObject holds the state of the ball object inheriting
/// relevant state data from GameObject. Contains some extra
/// functionality specific to Brekaout's ball object that
/// were too specific for within GameObject alone.
class BallObject : public GameObject
{
public:
	//ball state
	float		Radius;
	bool		Stuck;
	bool		Sticky, PassThrough;
	//constructor
	BallObject();
	BallObject(glm::vec2 pos, float radius, glm::vec2 velocity, Texture2D sprite);
	//moves the ball, keeping it constrained within the window bounds (Except bottom edge); returns new position
	glm::vec2 Move(float dt, unsigned int window_width);
	//resets the ball to original state with given position and velocity
	void	  Reset(glm::vec2 position, glm::vec2 velocity);
};
#endif

