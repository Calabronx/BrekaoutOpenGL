#include "Game.h"
#include "resource_manager.h"
#include "sprite_renderer.h"
#include "ball_object.h"
#include "particle_generator.h"
#include "postprocessor.h"

// Game related State data
SpriteRenderer		*Renderer;
GameObject			*Player;
ParticleGenerator	*Particles;
PostProcessor* Effects;

const glm::vec2 INITIAL_BALL_VELOCITY(100.0f, -350.0f);
const float BALL_RADIUS = 12.5f;

float ShakeTime = 0.0f;

typedef std::tuple<bool, Direction, glm::vec2> Collision;

BallObject* Ball;

Game::Game(unsigned int width, unsigned int height)
	: State(GAME_ACTIVE), Keys(), Width(width), Height(height)
{
}

Game::~Game()
{
	delete Renderer;
	delete Player;
	delete Ball;
	delete Particles;
	delete Effects;
}

void Game::Init()
{
	ResourceManager::LoadShader("shaders/sprite.vs", "shaders/sprite.frag", nullptr, "sprite");
	ResourceManager::LoadShader("shaders/particle.vs", "shaders/particle.frag", nullptr, "particle");
	ResourceManager::LoadShader("shaders/postprocessing.vs", "shaders/postprocessing.frag", nullptr, "postprocessing");

	glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width), static_cast<float>(this->Height),
		0.0f, -1.0f, 1.0f);
	ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
	ResourceManager::GetShader("sprite").Use().SetMatrix4("projection", projection);
	ResourceManager::GetShader("particle").Use().SetInteger("sprite", 0);
	ResourceManager::GetShader("particle").Use().SetMatrix4("projection", projection);
	//load textures
	ResourceManager::LoadTexture("textures/background.jpg", false, "background");
	ResourceManager::LoadTexture("textures/awesomeface.png", true, "face");
	ResourceManager::LoadTexture("textures/block.png", true, "block");
	ResourceManager::LoadTexture("textures/block_solid.png", true, "block_solid");
	ResourceManager::LoadTexture("textures/paddle.png", true, "paddle");
	ResourceManager::LoadTexture("textures/particle.png", true, "particle");
	// set render-specific controls
	Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
	Particles = new ParticleGenerator(ResourceManager::GetShader("particle"), ResourceManager::GetTexture("particle"), 800);
	Effects = new PostProcessor(ResourceManager::GetShader("postprocessing"), this->Width, this->Height);
	// load levels
	GameLevel one;
	GameLevel two;
	GameLevel three;
	GameLevel four;
	one.Load("levels/one.txt", this->Width, this->Height / 2);
	two.Load("levels/two.txt", this->Width, this->Height / 2);
	three.Load("levels/three.txt", this->Width, this->Height / 2);
	four.Load("levels/four.txt", this->Width, this->Height / 2);
	this->Levels.push_back(one);
	this->Levels.push_back(two);
	this->Levels.push_back(three);
	this->Levels.push_back(four);
	this->Level = 0;
	// configure game objects
	glm::vec2 playerPos = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f,this->Height - PLAYER_SIZE.y);
	Player = new GameObject(playerPos, PLAYER_SIZE, ResourceManager::GetTexture("paddle"));
	glm::vec2 ballPos = playerPos + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -BALL_RADIUS * 2.0f);
	Ball = new BallObject(ballPos, BALL_RADIUS, INITIAL_BALL_VELOCITY,ResourceManager::GetTexture("face"));
}

void Game::ProcessInput(float dt)
{
	if (this->State == GAME_ACTIVE)
	{
		float velocity = PLAYER_VELOCITY * dt;
		//move
		if (this->Keys[GLFW_KEY_A])
		{
			if (Player->Position.x >= 0.0f) {
				Player->Position.x -= velocity;
				if (Ball->Stuck)
					Ball->Position.x -= velocity;
			}
		}
		if (this->Keys[GLFW_KEY_D])
		{
			if (Player->Position.x <= this->Width - Player->Size.x) {
				Player->Position.x += velocity;
				if (Ball->Stuck)
					Ball->Position.x += velocity;
			}
		}
		if (this->Keys[GLFW_KEY_SPACE])
			Ball->Stuck = false;
	}

}

void Game::Update(float dt)
{
	Ball->Move(dt, this->Width);
	this->DoCollisions();

	Particles->Update(dt, *Ball, 2, glm::vec2(Ball->Radius / 2.0f));

	if (Ball->Position.y >= this->Height)
	{
		this->ResetLevel();
		this->ResetPlayer();
	}

	if(ShakeTime > 0.0f)
	{
		ShakeTime -= dt;
		if (ShakeTime <= 0.0f)
			Effects->Shake = false;
	}
}

void Game::Render()
{
	if (this->State == GAME_ACTIVE)
	{
		Effects->BeginRender();
		//draw background
		Renderer->DrawSprite(ResourceManager::GetTexture("background"),
			glm::vec2(0.0f, 0.0f), glm::vec2(this->Width, this->Height), 0.0f);
		//draw level
		this->Levels[this->Level].Draw(*Renderer);

		Player->Draw(*Renderer);	 //draw player
		Particles->Draw();  //draw particles
		Ball->Draw(*Renderer);	     //draw ball
		Effects->EndRender();
		Effects->Render((float)glfwGetTime());
	}
}

void Game::ResetLevel()
{
	if (this->Level == 0)
		this->Levels[0].Load("levels/one.txt", this->Width, this->Height / 2);
	else if(this->Level == 1)
		this->Levels[1].Load("levels/two.txt", this->Width, this->Height / 2);
	else if(this->Level == 2)
		this->Levels[2].Load("levels/three.txt", this->Width, this->Height / 2);
	else if(this->Level == 3)
		this->Levels[3].Load("levels/four.txt", this->Width, this->Height / 2);
}

//resets player//ball stats
void Game::ResetPlayer()
{
	Player->Size = PLAYER_SIZE;
	Player->Position = glm::vec2(this->Width / 2.0f - PLAYER_SIZE.x / 2.0f, this->Height - PLAYER_SIZE.y);
	Ball->Reset(Player->Position + glm::vec2(PLAYER_SIZE.x / 2.0f - BALL_RADIUS, -(BALL_RADIUS * 2.0f)), INITIAL_BALL_VELOCITY);
}

bool CheckCollision(GameObject& one, GameObject& two)
{
	bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
		two.Position.x + two.Size.x >= one.Position.x; // collision x-axis?
	bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
		two.Position.y + two.Size.y >= one.Position.y; // collision y-axis?
	//collision only if on both axes
	return collisionX && collisionY;
}

// calculates with direction a vector is facing (N,E,S or W)
Direction VectorDirection(glm::vec2 target)
{
	glm::vec2 compass[] = {
		glm::vec2(0.0f, 1.0f),  // up
		glm::vec2(1.0f, 0.0f),	// right
		glm::vec2(0.0f, -1.0f), //down
		glm::vec2(-1.0f, 0.0f)  //left
	};
	float max = 0.0f;
	unsigned int best_match = -1;
	for (unsigned int i = 0; i < 4; i++)
	{
		float dot_product = glm::dot(glm::normalize(target), compass[i]);
		if (dot_product > max)
		{
			max = dot_product;
			best_match = i;
		}
	}
	return (Direction)best_match;
}

Collision CheckCollision(BallObject& one, GameObject& two)
{
	glm::vec2 center(one.Position + one.Radius); // get center point circle first
	//calculate AABB info (center, half-extents)
	glm::vec2 aabb_half_extents(two.Size.x / 2.0f, two.Size.y / 2.0f);
	glm::vec2 aabb_center(
		two.Position.x + aabb_half_extents.x,
		two.Position.y + aabb_half_extents.y
	);
	glm::vec2 difference = center - aabb_center; // get difference vector between both centers
	glm::vec2 clamped = glm::clamp(difference, -aabb_half_extents, aabb_half_extents);
	glm::vec2 closest = aabb_center + clamped;//add a clamped value to AABB_center and we get the value of box closest to the circle
	difference = closest - center; // retrieve vector between center circle and closest point AABB and check if length <= radius

	if (glm::length(difference) <= one.Radius)
		return std::make_tuple(true, VectorDirection(difference), difference);
	else
		return std::make_tuple(false, UP, glm::vec2(0.0f, 0.0f));
}

void Game::DoCollisions()
{
	for (GameObject& box : this->Levels[this->Level].Bricks)
	{
		if (!box.Destroyed)
		{
			Collision collision = CheckCollision(*Ball, box);
			if (std::get<0>(collision)) // if collision is true
			{
				if (!box.IsSolid)// destroy block if not solid
					box.Destroyed = true;
				Direction dir = std::get<1>(collision);
				glm::vec2 diff_vector = std::get<2>(collision);
				if (dir == LEFT || dir == RIGHT) // horizontal collision
				{
					Ball->Velocity.x = -Ball->Velocity.x; // reverse horizontal velocity
					//relocate
					float penetration = Ball->Radius - std::abs(diff_vector.x);
					if (dir == LEFT)
						Ball->Position.x += penetration; // move ball to right
					else
						Ball->Position.x -= penetration; // move ball to left
				}
				else // vertical collision
				{
					ShakeTime = 0.05f;     //time effect
					Effects->Shake = true;// if block is solid, enable shake effect
					Ball->Velocity.y = -Ball->Velocity.y; // reverse vertical velocity
					float penetration = Ball->Radius - std::abs(diff_vector.y);
					if (dir == UP)
						Ball->Position.y -= penetration; // move ball back up
					else
						Ball->Position.y += penetration; // move ball back down
				}
			}
		}
	}
	Collision result = CheckCollision(*Ball, *Player);

	if (!Ball->Stuck && std::get<0>(result))
	{
		float centerBoard = Player->Position.x + Player->Size.x / 2.0f; // check where it hit the board, and change velocity based on where it hit the board
		float distance = (Ball->Position.x + Ball->Radius) - centerBoard;
		float percentage = distance / (Player->Size.x / 2.0f);
		// then move accordingly
		float strength = 2.0f;
		glm::vec2 oldVelocity = Ball->Velocity;
		Ball->Velocity.x = INITIAL_BALL_VELOCITY.x * percentage * strength;
		//Ball->Velocity.y = Ball->Velocity.y;
		Ball->Velocity.y = -1.0f * abs(Ball->Velocity.y);
		Ball->Velocity   = glm::normalize(Ball->Velocity) * glm::length(oldVelocity);
	}
}
