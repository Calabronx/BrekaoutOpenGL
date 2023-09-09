#pragma once

#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "shader.h"
#include "texture.h"
#include "game_object.h"

// represents a signle particle and its state
struct Particle {
	glm::vec2 Position, Velocity;
	glm::vec4 Color;
	float     Life;

	Particle()	: Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

/// ParticleGenerator acts as a container for rendering a large number of
/// particles by repeatedly spawning and updating particles and killing
/// them after a given amount of time.
class ParticleGenerator
{
public:
	ParticleGenerator(Shader shader, Texture2D texture, unsigned int amount);
	void Update(float dt, GameObject& object, unsigned int newParticles, glm::vec2 offset = glm::vec2(0.0f, 0.0f)); // updates all particles
	void Draw(); // render all particles
private:
	std::vector<Particle> particles;
	unsigned int amount;
	Shader shader;
	Texture2D texture;
	unsigned int VAO;
	void init(); // initializes buffer and vertex attributes
	unsigned int firstUnusedParticle(); // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
	void respawnParticle(Particle& particle, GameObject& object, glm::vec2 offset = glm::vec2(0.0f, 0.0f));
};
#endif

