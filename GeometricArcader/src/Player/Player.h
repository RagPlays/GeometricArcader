#ifndef PLAYER_H
#define PLAYER_H

#include <Engine.h>

#include "FlyFish.h"

#include "UI/General/ProgressBar.h"
#include "UI/SpeedController.h"

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

	// Getters / Setters //
	const TriVector& GetPosition() const;
	const TriVector Get2DPosition() const;
	void SetPosition(const TriVector& point);
	const glm::vec2& GetSize() const;
	
	void AddForce(const Motor& force);
	const float GetSpeed() const;

	void AddEnergy(float energy);
	float GetEnergyLevel() const;

	bool IsDead() const;

private:

	void UpdateEnergy(float deltaTime);
	void UpdateVelocity(float deltaTime);
	void UpdatePosition(float deltaTime);
	void UpdateCollision(const BorderCollision& coll);

	void UpdateEnergyBar();

	bool OnWindowResize(const Engine::WindowResizeEvent& e);

private:

	TriVector m_Position;
	glm::vec2 m_Size;
	Motor m_Velocity; // Transformer

	glm::vec4 m_EnergyRatioColor;

	const float m_EnergyGain;
	const float m_MaxEnergy;

	const float m_Acceleration;
	const float m_StartSpeed;
	const float m_MinimumSpeed;

	ProgressBar m_EnergyBar;
	SpeedController m_SpeedController;
};

#endif // !PLAYER_H