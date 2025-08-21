#include "Player.h"

#include "FlyFishTools/FlyFishUtils.h"
#include "Collision/BorderCollision.h"

using namespace Engine;

Player::Player()
	: m_Position{ 0.f, 0.f, 50.f } // 50 is the initial energy
	, m_Size{ 50.f, 50.f }
	, m_Velocity{}
	, m_EnergyRatioColor{}
	, m_EnergyGain{ 5.f }
	, m_MaxEnergy{ 100.f }
	, m_Acceleration{ 500.f } // units/s^2
	, m_StartSpeed{ 300.f }
	, m_MinimumSpeed{ 50.f }
	, m_EnergyBar{ UIAnchor::LeftBottom, { 30.f, 30.f }, { 500.f, 50.f } }
	, m_SpeedController{}
{
	ENGINE_ASSERT_MSG(m_StartSpeed > m_MinimumSpeed, "Base Speed must be greater than Minimum Speed");

	// Setup energy
	m_EnergyBar.SetValue(m_Position.e021());
	m_EnergyBar.SetMaxValue(m_MaxEnergy);

	m_Velocity = FlyFishUtils::GetTranslator(glm::vec3{ Random::Direction2D(), 0.f}, m_StartSpeed);
}

void Player::OnEvent(Event& e)
{
	m_EnergyBar.OnEvent(e);
}

void Player::Update(const BorderCollision& coll, float deltaTime)
{
	m_SpeedController.Update();

	UpdateEnergy(deltaTime);
	UpdateEnergyBar();
	
	UpdateVelocity(deltaTime);
	UpdatePosition(deltaTime);

	UpdateCollision(coll);
}

void Player::Render() const
{
	const TriVector position2D{ Get2DPosition() };
	
	Renderer2D::SetDrawColor(m_EnergyRatioColor);
	FlyFishUtils::DrawFillRect(position2D, glm::vec2{m_Size.x - 4.f, m_Size.y - 4.f});
	Renderer2D::SetDrawColor(Color::white);
	FlyFishUtils::DrawFillRect(position2D, m_Size);

	m_EnergyBar.Render();
	m_SpeedController.Render();
}

void Player::AddEnergy(float energy)
{
	// Add Energy
	m_Position.e021() += energy;

	// Clamp energy
	m_Position.e021() = std::clamp(m_Position.e021(), 0.f, m_MaxEnergy);
}

float Player::GetEnergyLevel() const
{
	return m_Position.e021();
}

bool Player::IsDead() const
{
	return m_EnergyBar.GetProgress() <= 0.f;
}

const TriVector& Player::GetPosition() const
{
	return m_Position;
}

const TriVector Player::Get2DPosition() const
{
	return TriVector{ m_Position.e032(), m_Position.e013(), 0.f, 1.f };
}

void Player::SetPosition(const TriVector& point)
{
	m_Position = point;
}

void Player::AddForce(const Motor& force)
{
	m_Velocity = force * m_Velocity;
}

const float Player::GetSpeed() const
{
	return m_Velocity.VNorm() * 2.f;
}

const glm::vec2& Player::GetSize() const
{
	return m_Size;
}

void Player::UpdateEnergy(float deltaTime)
{
	// Increase energy passively
	AddEnergy(m_EnergyGain * deltaTime);

	// Decrease energy based on speed
	const float speed{ GetSpeed() }; // Current Speed
	const float speedFactor{ (speed - m_MinimumSpeed) / ((m_StartSpeed + m_Acceleration) - m_MinimumSpeed) };
	const float energyDrain{ speedFactor * m_EnergyGain * deltaTime }; // The Faster you go the more drain there is
	AddEnergy(-energyDrain);
}

void Player::UpdateVelocity(float deltaTime)
{
	const auto moveState{ m_SpeedController.GetCurrentState() };
	if (moveState == SpeedController::ControllerState::NEUTRAL) return;

	const float vnorm{ m_Velocity.VNorm() }; // Safety Check
	if (vnorm < 1e-5f) return;
	 
	// Determine acceleration direction (Acceleration/Deceleration)
	const float accSign{ moveState == SpeedController::ControllerState::REVERSE ? -1.f : 1.f };

	// Scale With VNorm to get "forward", then Scale with the wanted speed gain and deltaTime
	const float totalScale{ (1.f / vnorm) * accSign * (m_Acceleration * 0.5f) * deltaTime };
	Motor accMotor{ FlyFishUtils::GetScaledTranslator(m_Velocity, totalScale) };

	// Accelerate / Decelerate the velocity
	AddForce(accMotor);

	// Enforce Minimum Speed
	const float newSpeed{ GetSpeed() };
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

void Player::UpdateEnergyBar()
{
	m_EnergyBar.SetValue(m_Position.e021());
	const float energyRatio{ m_Position.e021() / m_MaxEnergy };
	m_EnergyRatioColor = { 1.f - energyRatio, energyRatio, 0.f, 1.f };
	m_EnergyBar.SetValueColor(m_EnergyRatioColor);
}