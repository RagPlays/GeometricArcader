#ifndef GAME_H
#define GAME_H

#include <Engine.h>
#include "FlyFish.h"

class Game final
{
public:

	Game();
	~Game() = default;

	Game(const Game&) = delete;
	Game(Game&&) noexcept = delete;
	Game& operator=(const Game&) = delete;
	Game& operator=(Game&&) noexcept = delete;

	void Update(float deltaTime);
	void Render() const;

private:

	const Engine::Window& m_Window;
	TriVector m_BladePosition;
};

#endif // !GAME_H