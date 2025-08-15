#include "Player.h"

#include "FlyFishTools/FlyFishUtils.h"
#include "Collision/BorderCollision.h"

using namespace Engine;

Player::Player()
	: m_Position{ 0.f, 0.f, 0.f }
	, m_Velocity{}
	, m_Size{ 50.f, 150.f }
	, m_EnergyGain{ 5.f }
	, m_MaxEnergy{ 100.f }
	, m_EnergyBar{ UIAnchor::LeftBottom, { 30.f, 30.f }, { 500.f, 50.f } }
	, m_SpeedController{}
{

	// Setup energy
	m_Position.e021() = 50.f;
	m_EnergyBar.SetValue(0.f);
	m_EnergyBar.SetMaxValue(m_MaxEnergy);

	const BiVector xMoveLine{ 1.f, 0.f, 0.f, 0.f, 0.f, 0.f };
	const BiVector yMoveLine{ 0.f, 1.f, 0.f, 0.f, 0.f, 0.f };
	
	constexpr float maxSpeed{ 500.f };
	const bool randomVelDirX{ Random::Bool() };
	const bool randomVelDirY{ Random::Bool() };
	const float randomVelX{ Random::Range(300.f, maxSpeed) * (randomVelDirX ? -1.f : 1.f) };
	const float randomVelY{ Random::Range(300.f, maxSpeed) * (randomVelDirY ? -1.f : 1.f) };

	m_Velocity = Motor{ 1.f, randomVelX, randomVelY, 0.f, 0.f, 0.f, 0.f, 0.f };
}

void Player::OnEvent(Event& e)
{
}

void Player::Update(const BorderCollision& coll, float deltaTime)
{
	UpdateEnergy(deltaTime);
	m_EnergyBar.SetValue(m_Position.e021());
	m_SpeedController.Update();
	UpdateMovement(deltaTime);
	UpdateCollision(coll);
}

void Player::Render() const
{
	const TriVector position2D{ Get2DPosition() };
	const float energyRatio{ m_Position.e021() / m_MaxEnergy };
	Renderer2D::SetDrawColor(glm::vec4{ 1.f - energyRatio, energyRatio, 0.f, 1.f });
	FlyFishUtils::DrawFillRect(position2D, glm::vec2{m_Size.x - 4.f, m_Size.y - 4.f});
	Renderer2D::SetDrawColor(Color::white);
	FlyFishUtils::DrawFillRect(position2D, m_Size);

	m_EnergyBar.Render();
	m_SpeedController.Render();
}

// Getters //
const TriVector& Player::GetPosition() const
{
	return m_Position;
}

const TriVector Player::Get2DPosition() const
{
	return TriVector{ m_Position.e032(), m_Position.e013(), 0.f, 1.f };
}

const glm::vec2& Player::GetSize() const
{
	return m_Size;
}

void Player::UpdateEnergy(float deltaTime)
{
	// Increase By EnergyGain
	m_Position.e021() += m_EnergyGain * deltaTime; // Increase energy by gain per second

	// Clamp
	m_Position.e021() = std::clamp(m_Position.e021(), 0.f, m_MaxEnergy);
}

void Player::UpdateMovement(float deltaTime)
{
	Motor deltaTranslator{ FlyFishUtils::GetScaledTranslator(m_Velocity, deltaTime) };
	FlyFishUtils::Translate(m_Position, deltaTranslator);
}

void Player::UpdateCollision(const BorderCollision& coll)
{
	// Handles Collisions with the borders and returns all collisions
	CollisionsData collisions{ coll.HandleCollisions(m_Position, m_Size) };

	// If there are collisions, mirror velocity to the collided plane(s)
	if (!collisions.HasCollisions()) return;
	for (const auto& collision : collisions.Collisions)
	{
		if (!collision.HasCollision()) continue; // Extra check (should always be true here)
		const Vector& collidedPlane{ *collision.CollidedPlane };
		m_Velocity = MultiVector{ collidedPlane * m_Velocity * ~collidedPlane }.ToMotor();

		FlyFishUtils::ScaleTranslator(m_Velocity, 1.02f);
	}
}