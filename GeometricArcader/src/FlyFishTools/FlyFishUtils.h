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
    
    static glm::vec2 ToVec2(const TriVector& point);
	static glm::vec3 ToVec3(const TriVector& point);

	static float DistanceGLM(const TriVector& point0, const TriVector& point1);
    static float DistanceGA(const TriVector& point0, const TriVector& point1);
    static float SignedDistanceGA(const Vector& plane, const TriVector& point);

    static glm::vec3 GetDirectionGLM(const TriVector& from, const TriVector& to);
    static glm::vec3 GetDirectionGLM(const BiVector& directionLine);
    static glm::vec3 GetDirectionGA(const TriVector& from, const TriVector& to);
    static BiVector GetDirectionLineGA(const TriVector& from, const TriVector& to);
	static Vector GetDirectionPlane(const TriVector& from, const TriVector& to);

	static void ScaleTranslator(Motor& translator, float scale);
	static Motor GetScaledTranslator(const Motor& translator, float scale);
	static Motor GetTranslator(const glm::vec3& direction, float distance);
    static Motor GetTranslator(const BiVector& directionLine, float distance);
    static Motor GetTranslator(const Vector& directionPlane, float distance);
    static Motor GetTranslatorToPoint(const TriVector& point);

    // Plane Translation
    static void Translate(Vector& plane, float distance);

    // Point Translation
    static void Translate(TriVector& point, const Motor& translator);
    static void Translate(TriVector& point, const Vector& planeDirection, float distance);
    static TriVector GetTranslated(const TriVector& point, const Motor& translator);

    // Point Rotation
    static void RotateAroundPoint(TriVector& point, const TriVector& center, float angleDeg);

    // Projections Proj(x)m = (x | m) * ~m -> (x dot m) gep inverse(m)
	static Vector Projection(const Vector& plane, const Vector& referencePlane);
	static BiVector Projection(const BiVector& line, const Vector& referencePlane);
	static TriVector Projection(const TriVector& point, const Vector& referencePlane);
    static TriVector Projection(const TriVector& point, const BiVector& referenceLine);

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

    static void DrawCircle(const TriVector& point, float radius);

    static void DrawFilledCircle(const TriVector& point, float radius);

    static void DrawLine(const BiVector& line, float length);
    static void DrawLine(const TriVector& point0, const TriVector& point1);

    static void DrawRect(const TriVector& point, const glm::vec2& size);
    static void DrawFillRect(const TriVector& point, const glm::vec2& size);

};

#endif