#include "FlyFishUtils.h"

#include "FlyFish.h"

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

Motor FlyFishUtils::GetTranslator(const glm::vec3& direction, float distance)
{
	const float halfDist{ distance * 0.5f };
	return Motor
	{
		1.f,
		direction.x * halfDist,
		direction.y * halfDist,
		direction.z * halfDist,
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

void FlyFishUtils::Translate(TriVector& point, const glm::vec3& direction, float distance)
{
	// Normalize the direction vector
	const glm::vec3 dirN{ glm::normalize(direction) };

	// Take half negative distance (because of sandwich)
	const float d{ -distance * 0.5f };
	const Motor translator { 1.f, d * dirN.x, d * dirN.y, d * dirN.z, 0.f, 0.f, 0.f, 0.f };

	// Apply the Translation
	point = (translator * point * ~translator).Grade3();
}

void FlyFishUtils::Translate(TriVector& point, const Vector& planeDirection, float distance)
{
	Translate(point, glm::vec3{ planeDirection.e1(), planeDirection.e2(), planeDirection.e3() }, distance);
}

void FlyFishUtils::Translate(TriVector& point, const BiVector& lineDirection, float distance)
{
	const Motor translator{ Motor::Translation(distance, lineDirection) };
	point = (translator * point * ~translator).Grade3();
}

//void FlyFishUtils::Translate(TriVector& point, const Motor& translator)
//{
//	// Apply the Translation
//	point = (translator * point * ~translator).Grade3();
//}

// returns signed distance from TriVector point to Vector plane.
// Positive means point lies in direction of plane normal.
float FlyFishUtils::SignedDistanceToPlane(const Vector& plane, const TriVector& point)
{
	const float pn{ plane.Norm() };			// sqrt(a^2 + b^2 + c^2)
	const float pw{ point.Norm() };			// Scaler of e123 (usually 1.0f)
	if (pn == 0.0f || pw == 0.0f) return 0.0f;
	
	const float normProduct{ pn * pw };
	const float join{ plane & point };
	return (normProduct == 1.f) ? join : (join / normProduct);
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