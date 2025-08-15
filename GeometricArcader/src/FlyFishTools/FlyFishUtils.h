#ifndef FLYFISHUTILS_H
#define FLYFISHUTILS_H

#include <Engine.h>

class Vector;
class BiVector;
class TriVector;
class Motor;

class FlyFishUtils final
{
public:

    // * => (Gep)
	// | => (Dot)
    // ^ => (Meet)
	// & => (Join)
    // ~ => (Inverse)
    // ! => (Dual)

    // Variables //
	static const Vector e1;
	static const Vector e2;
	static const Vector e3;

    static const BiVector xAxis;
    static const BiVector yAxis;
    static const BiVector zAxis;

    // General Helpers //
    static bool IsVerticalPlane(const Vector& plane);
    static bool IsHorizontalPlane(const Vector& plane);

	static void ScaleTranslator(Motor& translator, float scale);
	static Motor GetScaledTranslator(const Motor& translator, float scale);

    // Plane translation
    static void Translate(Vector& plane, float distance);

    // Point translation
    static void Translate(TriVector& point, const Motor& translator);
    static void Translate(TriVector& point, const glm::vec3& direction, float distance);
    static void Translate(TriVector& point, const Vector& planeDirection, float distance);
    static void Translate(TriVector& point, const BiVector& lineDirection, float distance);

    static float SignedDistanceToPlane(const Vector& plane, const TriVector& point);

    // Projections Proj(x)m = (x | m) * ~m -> (x dot m) gep inverse(m)
	static Vector Projection(const Vector& plane, const Vector& referencePlane);
	static BiVector Projection(const BiVector& line, const Vector& referencePlane);
	static TriVector Projection(const TriVector& point, const Vector& referencePlane);

	// Rejections Rej(x)m = (x ^ m) * ~m -> (x meet m) gep inverse(m)
    // Plane = Proj(plane, referencePlane) + Rej(plane, referencePlane)
    // <=> Rej(plane, referencePlane) = plane - Proj(plane, referencePlane)
    static Vector Rejection(const Vector& plane, const Vector& referencePlane);
    static BiVector Rejection(const BiVector& line, const Vector& referencePlane);
    static TriVector Rejection(const TriVector& point, const Vector& referencePlane);

	//Reflection Ref(x)m = -m * x * ~m -> -m * x * inverse(m)
	static Vector Reflection(const Vector& plane, const Vector& mirrorPlane);
	static BiVector Reflection(const BiVector& line, const Vector& mirrorPlane);
	static TriVector Reflection(const TriVector& point, const Vector& mirrorPlane);

    // Rendering //
	static void DrawPoint(const TriVector& point);

    static void DrawFilledCircle(const TriVector& point, float radius);

    static void DrawLine(const BiVector& line, float length);
    static void DrawLine(const TriVector& point0, const TriVector& point1);

    static void DrawRect(const TriVector& point, float width, float height);
    static void DrawRect(const TriVector& point, const glm::vec2& size);
    static void DrawFillRect(const TriVector& point, float width, float height);
    static void DrawFillRect(const TriVector& point, const glm::vec2& size);

};

#endif