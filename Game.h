#ifndef  GAME_H
#define  GAME_H
#include <vector>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"

enum GameState {
	GAME_ACTIVE,
	GAME_MENU,
	GAME_WIN
};

class Game {
public:	
	GameState			   State;
	bool				   Keys[1024];
	std::vector<GameLevel> Levels;
	unsigned int		   Level;
	unsigned int		   Width, Height;
	Game(unsigned int width, unsigned int height);
	~Game();
	void Init();
	void ProcessInput(float dt);
	void Update(float dt);
	void Render();
};
#endif