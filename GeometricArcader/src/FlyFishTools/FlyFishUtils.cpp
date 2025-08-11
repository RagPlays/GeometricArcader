#include "FlyFishUtils.h"

#include <Engine.h>

#include "FlyFish.h"

const BiVector FlyFishUtils::xAxis{ BiVector{ 1, 0, 0, 0, 0, 0 } };
const BiVector FlyFishUtils::yAxis{ BiVector{ 0, 1, 0, 0, 0, 0 } };
const BiVector FlyFishUtils::zAxis{ BiVector{ 0, 0, 1, 0, 0, 0 } };

void FlyFishUtils::DrawLine(const BiVector& bv, float length)
{
	// Direction (Euclidean part)
	const glm::vec3 d{ bv.e23(), bv.e31(), bv.e12() };

	// Moment (vanishing part)
	const glm::vec3 m = { bv.e01(), bv.e02(), bv.e03() };

	// Get one point on the line
	glm::vec3 p0 = glm::cross(d, m);
	float denom = glm::dot(d, d);
	p0.x /= denom; p0.y /= denom; p0.z /= denom;

	// Normalize direction
	const glm::vec3 dirN = glm::normalize(d);

	const float halfLength = length * 0.5f;

	// Get finite endpoints
	const glm::vec3 p1
	{
		p0.x - dirN.x * halfLength,
		p0.y - dirN.y * halfLength,
		p0.z - dirN.z * halfLength
	};

	const glm::vec3 p2
	{
		p0.x + dirN.x * halfLength,
		p0.y + dirN.y * halfLength,
		p0.z + dirN.z * halfLength
	};

	// Draw the line
	Engine::Renderer2D::DrawLine(p1, p2);
}

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

void FlyFishUtils::DrawRect(const TriVector& tv, const glm::vec2& size)
{
	DrawRect(tv, size.x, size.y);
}

void FlyFishUtils::DrawFillRect(const TriVector& tv, float width, float height)
{
	const glm::vec3 pos{ tv[0], tv[1], tv[2] };
	const glm::vec2 size{ width, height };

    Engine::Renderer2D::DrawFilledRect(pos, size);
}

void FlyFishUtils::DrawFillRect(const TriVector& tv, const glm::vec2& size)
{
	DrawFillRect(tv, size.x, size.y);
}