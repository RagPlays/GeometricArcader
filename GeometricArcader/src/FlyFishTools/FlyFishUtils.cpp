#include "FlyFishUtils.h"

#include <Engine.h>

#include "FlyFish.h"

// Variables //
const Vector FlyFishUtils::e1Gen{ 0.f, 1.f, 0.f, 0.f };
const Vector FlyFishUtils::e2Gen{ 0.f, 0.f, 1.f, 0.f };
const Vector FlyFishUtils::e3Gen{ 0.f, 0.f, 0.f, 1.f };

const BiVector FlyFishUtils::xAxis{ BiVector{ 0.f, 0.f, 0.f, 1.f, 0.f, 0.f } };
const BiVector FlyFishUtils::yAxis{ BiVector{ 0.f, 0.f, 0.f, 0.f, 1.f, 0.f } };
const BiVector FlyFishUtils::zAxis{ BiVector{ 0.f, 0.f, 0.f, 0.f, 0.f, 1.f } };

// Calculations //

// returns signed distance from TriVector point to Vector plane.
// Positive means point lies in direction of plane normal.
float FlyFishUtils::SignedDistanceToPlane(const Vector& plane, const TriVector& point)
{
	const float join{ plane & point };	// Join
	const float pn{ plane.Norm() };		// sqrt(a^2 + b^2 + c^2)
	const float pw{ point.Norm() };      // Scaler of e123 (usually 1.0f)
	if (pn == 0.0f || pw == 0.0f) return 0.0f; // guard against degenerate inputs
	
	const float normProduct{ pn * pw };
	if (normProduct == 1.f) return join;
	else return join / (normProduct);
}

Vector FlyFishUtils::Projection(const Vector& plane, const Vector& referencePlane)
{
	return (plane | referencePlane) * ~referencePlane;
}

BiVector FlyFishUtils::Projection(const BiVector& line, const Vector& referencePlane)
{
	return ((line | referencePlane) * ~referencePlane).Grade2();
}

TriVector FlyFishUtils::Projection(const TriVector& point, const Vector& referencePlane)
{
	return ((point | referencePlane) * ~referencePlane).Grade3();
}

//Vector FlyFishUtils::Rejection(const Vector& plane, const Vector& referencePlane)
//{
//	
//}
//
//BiVector FlyFishUtils::Rejection(const BiVector& line, const Vector& referencePlane)
//{
//}
//
//TriVector FlyFishUtils::Rejection(const TriVector& point, const Vector& referencePlane)
//{
//	//auto meet = (point ^ referencePlane) * ~referencePlane; // this is a plane??
//}

// Rendering //
void FlyFishUtils::DrawPoint(const TriVector& point)
{
	const glm::vec3 pos{ point.e032(), point.e013(), point.e021() };
	Engine::Renderer2D::DrawPoint(pos);
}

void FlyFishUtils::DrawFilledCircle(const TriVector& point, float radius)
{
	const glm::vec3 pos{ point.e032(), point.e013(), point.e021() };
	Engine::Renderer2D::DrawFilledCircle(pos, radius);
}

void FlyFishUtils::DrawLine(const BiVector& line, float length)
{
	// Direction (Euclidean part)
	const glm::vec3 d{ line.e23(), line.e31(), line.e12() };

	// Moment (vanishing part)
	const glm::vec3 m{ line.e01(), line.e02(), line.e03() };

	// Get one point on the line
	const float denom{ glm::dot(d, d) };
	const glm::vec3 p0{ glm::cross(d, m) / denom };

	// Normalize direction
	const glm::vec3 dirN{ glm::normalize(d) };
	const float halfLength{ length * 0.5f }; // half length in each direction

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

void FlyFishUtils::DrawLine(const TriVector& point0, const TriVector& point1)
{
	const glm::vec3 pos0{ point0.e032(), point0.e013(), point0.e021() };
	const glm::vec3 pos1{ point1.e032(), point1.e013(), point1.e021() };
	Engine::Renderer2D::DrawLine(pos0, pos1);
}

void FlyFishUtils::DrawRect(const TriVector& pos, float width, float height)
{
	DrawRect(pos, glm::vec2{ width, height });
}

void FlyFishUtils::DrawRect(const TriVector& point, const glm::vec2& size)
{
	const glm::vec3 pos{ point.e032(), point.e013(), point.e021() };
	Engine::Renderer2D::DrawRect(pos, size);
}

void FlyFishUtils::DrawFillRect(const TriVector& pos, float width, float height)
{
	DrawFillRect(pos, glm::vec2{ width, height });
}

void FlyFishUtils::DrawFillRect(const TriVector& point, const glm::vec2& size)
{
	const glm::vec3 pos{ point.e032(), point.e013(), point.e021() };
	Engine::Renderer2D::DrawFilledRect(pos, size);
}