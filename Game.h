#ifndef  GAME_H
#define  GAME_H
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"
#include "power_up.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

enum Direction {
	UP,
	RIGHT,
	DOWN,
	LEFT
};


const glm::vec2 PLAYER_SIZE(100.0f, 20.0f); //initial size of the player
const float PLAYER_VELOCITY(500.0f);		//initial player velocity


class Game {
public:	
	GameState			   State;
	bool				   Keys[1024];
	std::vector<GameLevel> Levels;
	std::vector<PowerUp>   PowerUps;
	unsigned int		   Level;
	unsigned int		   Width, Height;
	Game(unsigned int width, unsigned int height);
	~Game();
	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
	void DoCollisions();
	void ResetLevel();
	void ResetPlayer();
	//powerups
	void SpawnPowerUps(GameObject& block);
	void UpdatePowerUps(float dt);
};
#endif