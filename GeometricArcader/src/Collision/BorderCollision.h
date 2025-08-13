#ifndef BORDERCOLLISION_H
#define BORDERCOLLISION_H

#include <array>
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

private:

	std::array<Vector, 4> m_BorderPlanes; // Planes for each border (left, right, top, bottom)

};

#endif // !BORDERCOLLISION_H