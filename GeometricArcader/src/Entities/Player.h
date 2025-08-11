#ifndef PLAYER_H
#define PLAYER_H

#include <Engine.h>

#include "FlyFish.h"

class Player final
{
public:

	Player();
	~Player() = default;

	Player(const Player&) = delete;
	Player(Player&&) noexcept = delete;
	Player& operator=(const Player&) = delete;
	Player& operator=(Player&&) noexcept = delete;

	void OnEvent(Engine::Event& e);

	void Update(float deltaTime);
	void Render() const;

private:

	bool OnKeyReleased(Engine::KeyReleasedEvent& event);

private:

	TriVector m_Position;
	glm::vec2 m_Size;

};

#endif // !PLAYER_H