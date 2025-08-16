#include "Enemy.h"

#include "FlyFishTools/FlyFishUtils.h"
#include "Collision/BorderCollision.h"

using namespace Engine;

Enemy::Enemy(const TriVector& position, const glm::vec2& size)
	: m_Position{ position }
	, m_Size{ size }
	, m_MoveSpeed{ 150.f }
{
	m_Velocity = FlyFishUtils::GetTranslator(glm::vec3{ Random::Direction2D(), 0.f }, m_MoveSpeed);
}

void Enemy::Update(const BorderCollision& coll, float deltaTime)
{
	UpdatePosition(deltaTime);

	UpdateCollision(coll);
}

void Enemy::Render() const
{
	Renderer2D::SetDrawColor(glm::vec4{ 0.6f, 0.f, 0.f, 1.f});
	FlyFishUtils::DrawFillRect(m_Position, glm::vec2{ m_Size.x - 4.f, m_Size.y - 4.f });
	/*Renderer2D::SetDrawColor(Color::lightGray);
	FlyFishUtils::DrawFillRect(m_Position, m_Size);*/
}

const TriVector& Enemy::GetPosition() const
{
	return m_Position;
}

const glm::vec2& Enemy::GetSize() const
{
	return m_Size;
}

void Enemy::UpdatePosition(float deltaTime)
{
	Motor deltaTranslator{ FlyFishUtils::GetScaledTranslator(m_Velocity, deltaTime) };
	FlyFishUtils::Translate(m_Position, deltaTranslator);
}

void Enemy::UpdateCollision(const BorderCollision& coll)
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