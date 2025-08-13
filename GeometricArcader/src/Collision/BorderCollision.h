#ifndef BORDERCOLLISION_H
#define BORDERCOLLISION_H

#include <Engine.h>

#include "FlyFish.h"

class Player;

class BorderCollision final
{
public:

	explicit BorderCollision(uint32_t borderOffset = 0.f);
	~BorderCollision() = default;

	BorderCollision(const BorderCollision&) = delete;
	BorderCollision(BorderCollision&&) noexcept = delete;
	BorderCollision& operator=(const BorderCollision&) = delete;
	BorderCollision& operator=(BorderCollision&&) noexcept = delete;

	void OnEvent(Engine::Event& e);

	void Render() const;

	bool IsPlayerColliding(const Player& player) const;

private:

	void UpdateBorderPlanes(uint32_t width, uint32_t height);

	// Window Events //
	bool OnWindowResized(Engine::WindowResizeEvent& e);

private:

	const uint32_t m_BorderOffset;
	float m_OffsetX;
	float m_OffsetY;
	std::array<Vector, 4> m_BorderPlanes; // Planes for each border (left, right, top, bottom)

};

#endif // !BORDERCOLLISION_H