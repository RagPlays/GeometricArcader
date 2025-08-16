#ifndef ENEMY_H
#define ENEMY_H

#include <Engine.h>

#include "FlyFish.h"

class BorderCollision;

class Enemy final
{
public:

	explicit Enemy(const TriVector& position, const glm::vec2& size);
	~Enemy() = default;

	/*Enemy(const Enemy&) = delete;
	Enemy(Enemy&&) noexcept = delete;
	Enemy& operator=(const Enemy&) = delete;
	Enemy& operator=(Enemy&&) noexcept = delete;*/

	void Update(const BorderCollision& coll, float deltaTime);
	void Render() const;

	// Getters //
	const TriVector& GetPosition() const;
	const glm::vec2& GetSize() const;

private:

	void UpdatePosition(float deltaTime);
	void UpdateCollision(const BorderCollision& coll);

private:

	TriVector m_Position;
	const glm::vec2 m_Size;
	Motor m_Velocity;
	const float m_MoveSpeed;

};

#endif // !ENEMY_H
