#ifndef FLYFISHUTILS_H
#define FLYFISHUTILS_H

#include <Engine.h>

class Vector;
class BiVector;
class TriVector;

class FlyFishUtils
{
public:

    // * => (Gep)
	// | => (Dot)
    // ^ => (Meet)
	// & => (Join)
    // ~ => (Inverse)
    // ! => (Dual)

    // Variables //
	static const Vector e1Gen;
	static const Vector e2Gen;
	static const Vector e3Gen;

    static const BiVector xAxis;
    static const BiVector yAxis;
    static const BiVector zAxis;

    // Calculations //
    static float SignedDistanceToPlane(const Vector& plane, const TriVector& point);

    // Projections Proj(x)m = (x | m) * ~m -> "(x dot m) gep inverse(m)"
	static Vector Projection(const Vector& plane, const Vector& referencePlane);
	static BiVector Projection(const BiVector& line, const Vector& referencePlane);
	static TriVector Projection(const TriVector& point, const Vector& referencePlane);

	// Rejections Rej(x)m = (x ^ m) * ~m -> "(x meet m) gep inverse(m)"
    /*static Vector Rejection(const Vector& plane, const Vector& referencePlane);
    static BiVector Rejection(const BiVector& line, const Vector& referencePlane);
    static TriVector Rejection(const TriVector& point, const Vector& referencePlane);*/


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