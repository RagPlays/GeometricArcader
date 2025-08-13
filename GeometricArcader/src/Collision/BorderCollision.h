#ifndef BORDERCOLLISION_H
#define BORDERCOLLISION_H

#include <Engine.h>

#include "FlyFish.h"

class BorderCollision final
{
public:

	explicit BorderCollision(float borderOffset = 0.f);
	~BorderCollision() = default;

	BorderCollision(const BorderCollision&) = delete;
	BorderCollision(BorderCollision&&) noexcept = delete;
	BorderCollision& operator=(const BorderCollision&) = delete;
	BorderCollision& operator=(BorderCollision&&) noexcept = delete;

	void OnEvent(Engine::Event& e);

private:

	bool OnWindowResized(Engine::WindowResizeEvent& e);

private:

	const float m_borderOffset;
	std::array<Vector, 4> m_BorderPlanes; // Planes for each border (left, right, top, bottom)

};

#endif // !BORDERCOLLISION_H