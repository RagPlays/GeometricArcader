#include "FlyFishUtils.h"

#include <Engine.h>

#include "FlyFish.h"

const BiVector FlyFishUtils::xAxis{ BiVector{ 1, 0, 0, 0, 0, 0 } };
const BiVector FlyFishUtils::yAxis{ BiVector{ 0, 1, 0, 0, 0, 0 } };
const BiVector FlyFishUtils::zAxis{ BiVector{ 0, 0, 1, 0, 0, 0 } };

//void FlyFishUtils::DrawLine(const BiVector& bv)
//{
//	const glm::vec3 p1{ bv[0], bv[1], bv[2] };
//	const glm::vec3 p2{ bv[3], bv[4], bv[5] };
//	Engine::Renderer2D::DrawLine(p1, p2);
//}
//
//void FlyFishUtils::DrawLine(const TriVector& tv1, const TriVector& tv2)
//{
//	const glm::vec3 p1{ tv1[0], tv1[1], tv1[2] };
//	const glm::vec3 p2{ tv2[0], tv2[1], tv2[2] };
//	Engine::Renderer2D::DrawLine(p1, p2);
//}

void FlyFishUtils::DrawRect(const TriVector& tv, float width, float height)
{
	const glm::vec3 pos{ tv[0], tv[1], tv[2] };
	const glm::vec2 size{ width, height };

	Engine::Renderer2D::DrawRect(pos, size);
}

void FlyFishUtils::DrawFillRect(const TriVector& tv, float width, float height)
{
	const glm::vec3 pos{ tv[0], tv[1], tv[2] };
	const glm::vec2 size{ width, height };

    Engine::Renderer2D::DrawFilledRect(pos, size);
}