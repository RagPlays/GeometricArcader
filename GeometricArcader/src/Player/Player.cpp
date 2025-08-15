#include "Player.h"

#include "FlyFishTools/FlyFishUtils.h"
#include "Collision/BorderCollision.h"

using namespace Engine;

Player::Player()
	: m_Position{ 0.f, 0.f, 50.f } // 50 is the initial energy
	, m_Size{ 50.f, 50.f }
	, m_Velocity{}
	, m_EnergyGain{ 5.f }
	, m_MaxEnergy{ 100.f }
	, m_Acceleration{ 1000.f } // units/s^2
	, m_BaseSpeed{ 500.f }
	, m_MinimumSpeed{ 100.f }
	, m_EnergyBar{ UIAnchor::LeftBottom, { 30.f, 30.f }, { 500.f, 50.f } }
	, m_SpeedController{}
{
	ENGINE_ASSERT_MSG(m_BaseSpeed > m_MinimumSpeed, "Base Speed must be greater than Minimum Speed");

	// Setup energy
	m_EnergyBar.SetValue(m_Position.e021());
	m_EnergyBar.SetMaxValue(m_MaxEnergy);

	m_Velocity = FlyFishUtils::GetTranslator(glm::vec3{ 0.f, 1.f, 0.f}, m_BaseSpeed);
}

void Player::OnEvent(Event& e)
{
}

void Player::Update(const BorderCollision& coll, float deltaTime)
{
	m_SpeedController.Update();
	UpdateEnergy(deltaTime);
	m_EnergyBar.SetValue(m_Position.e021());
	
	UpdateVelocity(deltaTime);
	UpdatePosition(deltaTime);

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

void Player::UpdateVelocity(float deltaTime)
{
	const auto moveState{ m_SpeedController.GetCurrentState() };
	if (moveState == SpeedController::ControllerState::NEUTRAL) return;

	// Determine acceleration direction (Acceleration/Deceleration)
	const float accSign{ moveState == SpeedController::ControllerState::REVERSE ? -1.f : 1.f };
	
	// Scale To VNorm to get "forward" then Scale to the wanted speed gain * deltaTime
	float totalScale{ (1.f / m_Velocity.VNorm()) * accSign * (m_Acceleration * 0.5f) * deltaTime };
	Motor accMotor{ m_Velocity };
	FlyFishUtils::ScaleTranslator(accMotor, totalScale);

	// Accelerate / Decelerate the velocity
	m_Velocity = accMotor * m_Velocity;

	// Enforce Minimum Speed
	float newSpeed{ m_Velocity.VNorm() };
	if (newSpeed < m_MinimumSpeed)
	{
		FlyFishUtils::ScaleTranslator(m_Velocity, m_MinimumSpeed / newSpeed);
	}
}

void Player::UpdatePosition(float deltaTime)
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
	}
}