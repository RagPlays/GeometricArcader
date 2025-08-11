#ifndef GAME_H
#define GAME_H

#include <Engine.h>

#include "Entities/Player.h"

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

	void DrawAxes() const;
	void DrawLineTest() const;

private:

	const Engine::Window& m_Window;
	Player m_Player;
	
};

#endif // !GAME_H