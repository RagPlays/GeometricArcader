#ifndef FLYFISHUTILS_H
#define FLYFISHUTILS_H

#include <Engine.h>

class Vector;
class BiVector;
class TriVector;

class FlyFishUtils
{
public:

	static const Vector e1Gen;
	static const Vector e2Gen;
	static const Vector e3Gen;

    static const BiVector xAxis;
    static const BiVector yAxis;
    static const BiVector zAxis;

    static void DrawLine(const BiVector& bv, float length);
    static void DrawLine(const TriVector& p0, const TriVector& p1);

    static void DrawRect(const TriVector& pos, float width, float height);
    static void DrawRect(const TriVector& pos, const glm::vec2& size);
    static void DrawFillRect(const TriVector& pos, float width, float height);
    static void DrawFillRect(const TriVector& pos, const glm::vec2& size);

    static float SignedDistanceToPlane(const Vector& plane, const TriVector& pt);
};

#endif