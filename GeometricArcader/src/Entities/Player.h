#ifndef PLAYER_H
#define PLAYER_H

#include <Engine.h>

#include "FlyFish.h"

class BorderCollision;

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

	void Update(const BorderCollision& coll, float deltaTime);
	void Render() const;

	// Getters //
	const TriVector& GetPosition() const;
	const glm::vec2& GetSize() const;

private:

	void UpdateMovement(float deltaTime);
	void UpdateCollision(const BorderCollision& coll);

	bool OnKeyReleased(Engine::KeyReleasedEvent& event);

private:

	TriVector m_Position;
	glm::vec2 m_Size;

};

#endif // !PLAYER_H