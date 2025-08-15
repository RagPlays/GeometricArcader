#ifndef PLAYER_H
#define PLAYER_H

#include <Engine.h>

#include "FlyFish.h"

#include "EnergyBar.h"
#include "SpeedController.h"

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
	const TriVector Get2DPosition() const;
	const glm::vec2& GetSize() const;

private:

	void UpdateEnergy(float deltaTime);
	void UpdateVelocity(float deltaTime);
	void UpdatePosition(float deltaTime);
	void UpdateCollision(const BorderCollision& coll);

private:

	TriVector m_Position;
	glm::vec2 m_Size;
	Motor m_Velocity; // Transformer

	const float m_EnergyGain;
	const float m_MaxEnergy;

	const float m_Acceleration;
	const float m_BaseSpeed;
	const float m_MinimumSpeed;

	ProgressBar m_EnergyBar;
	SpeedController m_SpeedController;
};

#endif // !PLAYER_H