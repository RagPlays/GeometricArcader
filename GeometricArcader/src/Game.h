#ifndef GAME_H
#define GAME_H

#include <Engine.h>

#include "Collision/BorderCollision.h"
#include "Entities/Player/Player.h"
#include "Entities/Enemy/Enemy.h"

class Game final
{
public:

	Game();
	~Game() = default;

	Game(const Game&) = delete;
	Game(Game&&) noexcept = delete;
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) noexcept = delete;

	void OnEvent(Engine::Event& e);

	void Update(float deltaTime);
	void Render() const;

private:

	void CreateEnemies();

private:

	Engine::Window& m_Window;
	BorderCollision m_BorderCollision;
	Player m_Player;

	std::vector<Enemy> m_Enemies;
	
};

#endif // !GAME_H