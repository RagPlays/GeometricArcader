#include "FlyFishUtils.h"

#include "FlyFish.h"
#include <cmath> // for std::isfinite

// Variables //
const Vector FlyFishUtils::e1{ 0.f, 1.f, 0.f, 0.f };
const Vector FlyFishUtils::e2{ 0.f, 0.f, 1.f, 0.f };
const Vector FlyFishUtils::e3{ 0.f, 0.f, 0.f, 1.f };

const BiVector FlyFishUtils::xAxis{ BiVector{ 0.f, 0.f, 0.f, 1.f, 0.f, 0.f } };
const BiVector FlyFishUtils::yAxis{ BiVector{ 0.f, 0.f, 0.f, 0.f, 1.f, 0.f } };
const BiVector FlyFishUtils::zAxis{ BiVector{ 0.f, 0.f, 0.f, 0.f, 0.f, 1.f } };

// Checks if the plane is vertical, meaning it has a non-zero e1 component and zero e2 and e3 components.
bool FlyFishUtils::IsVerticalPlane(const Vector& plane)
{
	constexpr float epsilon{ 1e-6f };
	// Plane should be close to 1*e1 + 0*e2 + 0*e3
	return (std::fabs(plane.e1()) > epsilon && std::fabs(plane.e2()) < epsilon && std::fabs(plane.e3()) < epsilon);
}

// Checks if the plane is vertical, meaning it has a non-zero e2 component and zero e1 and e3 components.
bool FlyFishUtils::IsHorizontalPlane(const Vector& plane)
{
	constexpr float epsilon{ 1e-6f };
	// Plane should be close to 0*e1 + 1*e2 + 0*e3
	return (std::fabs(plane.e1()) < epsilon && std::fabs(plane.e2()) > epsilon && std::fabs(plane.e3()) < epsilon);
}

glm::vec2 FlyFishUtils::ToVec2(const TriVector& point)
{
	// point at infinity
	if (point.e123() == 0.0f) return glm::vec2{ std::numeric_limits<float>::infinity() };

	const float invW{ 1.0f / point.e123() }; // Mostly stays 1.f
	return glm::vec2{ point.e032() * invW, point.e013() * invW };
}

glm::vec3 FlyFishUtils::ToVec3(const TriVector& point)
{
	// point at infinity
	if (point.e123() == 0.0f) return glm::vec3{ std::numeric_limits<float>::infinity() };

	const float invW{ 1.0f / point.e123() }; // Mostly stays 1.f
	return glm::vec3{ point.e032() * invW, point.e013() * invW, point.e021() * invW };
}

TriVector FlyFishUtils::GetPoint2D(const TriVector& point)
{
	return TriVector{ point.e032(), point.e013() , 0.f, 1.f };
}

float FlyFishUtils::DistanceGLM(const TriVector& point0, const TriVector& point1)
{
	const glm::vec3 pos0{ ToVec3(point0) };
	const glm::vec3 pos1{ ToVec3(point1) };

	// If either is infinite, return infinite
	if (
		!glm::all(glm::vec<3, bool>{ std::isfinite(pos0.x), std::isfinite(pos0.y), std::isfinite(pos0.z) }) ||
		!glm::all(glm::vec<3, bool>{ std::isfinite(pos1.x), std::isfinite(pos1.y), std::isfinite(pos1.z) })
		)
	{
		ENGINE_WARN("FlyFishUtils::DistanceGLM - Asked distance when point was infinte");
		return std::numeric_limits<float>::infinity();
	}

	return glm::distance(pos0, pos1);
}

float FlyFishUtils::DistanceGA(const TriVector& point0, const TriVector& point1)
{
	if (point0.e123() == 0.0f)
	{
		ENGINE_WARN("FlyFishUtils::DistanceGA - Asked distance when point was infinte");
		return std::numeric_limits<float>::infinity();
	}

	return (point0 & point1).Norm();
}

// Positive means point lies in direction of plane normal.
float FlyFishUtils::SignedDistanceGA(const Vector& plane, const TriVector& point)
{
	if (point.e123() == 0.0f || !(plane.e1() != 0.0f || plane.e2() != 0.0f || plane.e3() != 0.0f))
	{
		ENGINE_WARN("FlyFishUtils::SignedDistanceGA - Asked distance when plane or point was infinte");
		return std::numeric_limits<float>::infinity();
	}

	return point.Normalized() & plane.Normalized();
}

glm::vec3 FlyFishUtils::GetDirectionGLM(const TriVector& from, const TriVector& to)
{
	ENGINE_ASSERT_MSG(from.e123() != 0.f && to.e123() != 0.f, "Cant find direction of infinite points");

	const glm::vec3 p1{ ToVec3(from) };
	const glm::vec3 p2{ ToVec3(to) };
	const glm::vec3 dir{ glm::normalize(p2 - p1) };

	return dir;
}

glm::vec3 FlyFishUtils::GetDirectionGLM(const BiVector& directionLine)
{
	return glm::vec3{ directionLine.e23(), directionLine.e31(), directionLine.e12() };
}

glm::vec3 FlyFishUtils::GetDirectionGA(const TriVector& from, const TriVector& to)
{
	ENGINE_ASSERT_MSG(from.e123() != 0.f && to.e123() != 0.f, "Cant find direction of infinite points");

	BiVector dirLine{ (from & to).Normalized() };
	return glm::vec3{ dirLine.e23(), dirLine.e31(), dirLine.e12() };
}

BiVector FlyFishUtils::GetDirectionLineGA(const TriVector& from, const TriVector& to)
{
	ENGINE_ASSERT_MSG(from.e123() != 0.f && to.e123() != 0.f, "Cant find direction of infinite points");

	return (from & to).Normalized();
}

Vector FlyFishUtils::GetDirectionPlane(const TriVector& from, const TriVector& to)
{
	const glm::vec3 dir{ GetDirectionGA(from, to) };
	return Vector{ 0.0f, dir.x, dir.y, dir.z }; // Plane through origin with only a direction
}

void FlyFishUtils::ScaleTranslator(Motor& translator, float scale)
{
	translator.e01() *= scale;
	translator.e02() *= scale;
	translator.e03() *= scale;
}

Motor FlyFishUtils::GetScaledTranslator(const Motor& translator, float scale)
{
	Motor scaledTranslator{ translator };
	scaledTranslator.e01() *= scale;
	scaledTranslator.e02() *= scale;
	scaledTranslator.e03() *= scale;
	return scaledTranslator;
}

Motor FlyFishUtils::GetTranslator(const glm::vec3& direction, float distance) // Only used to create a random start direction for player (that why glm::vec3)
{
	const glm::vec3 dirN{ glm::normalize(direction) };
	const float d{ -distance * 0.5f };

	return Motor
	{
		1.f,
		direction.x * d,
		direction.y * d,
		direction.z * d,
		0.f,
		0.f,
		0.f,
		0.f
	};
}

Motor FlyFishUtils::GetTranslator(const BiVector& directionLine, float distance)
{
	const BiVector dirN{ directionLine.Normalized() };
	const float d{ -distance * 0.5f };

	return Motor
	{
		1.f,
		dirN.e23() * d,
		dirN.e31() * d,
		dirN.e12() * d,
		0.f,
		0.f,
		0.f,
		0.f
	};
}

Motor FlyFishUtils::GetTranslator(const Vector& directionPlane, float distance)
{
	const Vector dirN{ directionPlane.Normalized() };
	const float d{ -distance * 0.5f };

	return Motor
	{
		1.f,
		dirN.e1() * d,
		dirN.e2() * d,
		dirN.e3() * d,
		0.f,
		0.f,
		0.f,
		0.f
	};
}

Motor FlyFishUtils::GetTranslatorToPoint(const TriVector& point)
{
	return Motor
	{
		1.f,
		point.e032() * -0.5f,
		point.e013() * -0.5f,
		point.e021() * -0.5f,
		0.f,
		0.f,
		0.f,
		0.f
	};
}

void FlyFishUtils::Translate(Vector& plane, float distance)
{
	plane.e0() -= distance; // (-) because n = n^ + n0(-e0) -> (-e0)
}

void FlyFishUtils::Translate(TriVector& point, const Motor& translator)
{
	point = (translator * point * ~translator).Grade3();
}

void FlyFishUtils::Translate(TriVector& point, const Vector& planeDirection, float distance)
{
	// Get Translator in the direction of the plane over a distance
	const Motor translator{ GetTranslator(planeDirection.Normalized(), distance) };

	// Translate point with translator
	Translate(point, translator);
}

TriVector FlyFishUtils::GetTranslated(const TriVector& point, const Motor& translator)
{
	return (translator * point * ~translator).Grade3();
}

float FlyFishUtils::GetAngleBetweenLinesGA(const BiVector& line0, const BiVector& line1)
{
	// L . K = - |L| |K| cos(angle)
	// angle = acos(-(L . K) / (|L| |K|))
	// since both lines are normalized |L| = |K| = 1
	// => angle = acos(- L . K)

	const BiVector line0N{ line0.Normalized() };
	const BiVector line1N{ line1.Normalized() };
	const float dotProduct{ line0N | line1N };
	return std::acos(-dotProduct);
}

void FlyFishUtils::Rotate(TriVector& point, const Motor& rotator)
{
	point = (rotator * point * ~rotator).Grade3();
}

void FlyFishUtils::RotateAroundPoint(TriVector& point, const TriVector& center, float angleDeg)
{
	// Rotation motor around origin
	const Motor rotator{ Motor::Rotation(angleDeg, FlyFishUtils::zAxis) };

	// Translation motor to move center to origin
	const Motor translator{ FlyFishUtils::GetTranslatorToPoint(center) };

	// Compose motor: translate to center, rotate, back
	const Motor translatedRotator{ translator  * rotator * ~translator };

	// Apply sandwich product
	point = (translatedRotator * point * ~translatedRotator).Grade3();
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

TriVector FlyFishUtils::Projection(const TriVector& point, const BiVector& referenceLine)
{
	return ((point | referenceLine) * ~referenceLine).Grade3();
}

Vector FlyFishUtils::Rejection(const Vector& plane, const Vector& referencePlane)
{
	return (plane - Projection(plane, referencePlane));
}

BiVector FlyFishUtils::Rejection(const BiVector& line, const Vector& referencePlane)
{
	return (line - Projection(line, referencePlane));
}

TriVector FlyFishUtils::Rejection(const TriVector& point, const Vector& referencePlane)
{
	return (point - Projection(point, referencePlane));
}

Vector FlyFishUtils::Reflection(const Vector& plane, const Vector& mirrorPlane)
{
	return (-mirrorPlane * plane * ~mirrorPlane).Grade1();
}

BiVector FlyFishUtils::Reflection(const BiVector& line, const Vector& mirrorPlane)
{
	return (-mirrorPlane * line * ~mirrorPlane).Grade2();
}

TriVector FlyFishUtils::Reflection(const TriVector& point, const Vector& mirrorPlane)
{
	return (-mirrorPlane * point * ~mirrorPlane).Grade3();
}

// Rendering //
void FlyFishUtils::DrawPoint(const TriVector& point)
{
	const glm::vec3 pos{ point.e032(), point.e013(), point.e021() };
	Engine::Renderer2D::DrawPoint(pos);
}

void FlyFishUtils::DrawCircle(const TriVector& point, float radius)
{
	const glm::vec3 pos{ point.e032(), point.e013(), point.e021() };
	Engine::Renderer2D::DrawCircle(pos, radius);
}

void FlyFishUtils::DrawFilledCircle(const TriVector& point, float radius)
{
	const glm::vec3 pos{ point.e032(), point.e013(), point.e021() };
	Engine::Renderer2D::DrawFilledCircle(pos, radius);
}

void FlyFishUtils::DrawLine(const BiVector& line, float length)
{
	// Base point: projection of origin onto line
	const TriVector origin{ 0.f, 0.f, 0.f, 1.f };
	const TriVector projectionPoint{ Projection(origin, line) };

	// Translate point over the line 
	const float halfLength{ length * 0.5f };
	const Motor translatorP1{ GetTranslator(line, halfLength) };
	const Motor translatorP2{ GetTranslator(line, -halfLength) }; // Or -line, halfLength
	
	const TriVector linePoint0{ GetTranslated(projectionPoint, translatorP1) };
	const TriVector linePoint1{ GetTranslated(projectionPoint, translatorP2) };

	// Convert only once at the very end for rendering
	Engine::Renderer2D::DrawLine(ToVec3(linePoint0), ToVec3(linePoint1));
}

void FlyFishUtils::DrawLine(const TriVector& point0, const TriVector& point1)
{
	Engine::Renderer2D::DrawLine({ point0.e032(), point0.e013(), point0.e021() }, { point1.e032(), point1.e013(), point1.e021() });
}

void FlyFishUtils::DrawRect(const TriVector& point, const glm::vec2& size)
{
	Engine::Renderer2D::DrawRect({ point.e032(), point.e013(), point.e021() }, size);
}

void FlyFishUtils::DrawFillRect(const TriVector& point, const glm::vec2& size)
{
	Engine::Renderer2D::DrawFilledRect({ point.e032(), point.e013(), point.e021() }, size);
}

void FlyFishUtils::DrawFillRect(const TriVector& point, const glm::vec2& size, float angleRad)
{
	Engine::Renderer2D::DrawFilledRect({ point.e032(), point.e013(), point.e021() }, size, angleRad);
}