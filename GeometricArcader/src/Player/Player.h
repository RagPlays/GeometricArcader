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
	void UpdateMovement(float deltaTime);
	void UpdateCollision(const BorderCollision& coll);

private:

	TriVector m_Position;
	Motor m_Velocity; // Transformer
	glm::vec2 m_Size;

	const float m_EnergyGain;
	const float m_MaxEnergy;

	ProgressBar m_EnergyBar;
	SpeedController m_SpeedController;
};

#endif // !PLAYER_H