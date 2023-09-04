#include "Game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"

SpriteRenderer* Renderer;

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
	delete Renderer;
}

void Game::Init()
{
	ResourceManager::LoadShader("shaders/sprite_shader.vs", "shaders/sprite_frs.frs", nullptr, "sprite");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height),
		0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");

}

void Game::ProcessInput(float dt)
{
}

void Game::Update(float dt)
{
}

void Game::Render()
{
	//esta linea no compila
	//Renderer->DrawSprite(ResourceManager::GetTexture("face"), glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
	Texture2D myTexture;
	myTexture = ResourceManager::GetTexture("face");
	Renderer->DrawSprite(myTexture, glm::vec2(200.0f, 200.0f), glm::vec2(300.0f, 400.0f), 45.0f, glm::vec3(0.0f, 1.0f, 0.0f));
}
