#ifndef FLYFISHUTILS_H
#define FLYFISHUTILS_H

#include <Engine.h>

class Vector;
class BiVector;
class TriVector;

class FlyFishUtils
{
public:

    static const BiVector xAxis;
    static const BiVector yAxis;
    static const BiVector zAxis;

    static void DrawLine(const BiVector& bv, float length);
    //static void DrawLine(const TriVector& p1, const TriVector& p2);

    static void DrawRect(const TriVector& tv, float width, float height);
    static void DrawRect(const TriVector& tv, const glm::vec2& size);
    static void DrawFillRect(const TriVector& tv, float width, float height);
    static void DrawFillRect(const TriVector& tv, const glm::vec2& size);
};

#endif