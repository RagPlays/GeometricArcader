#ifndef FLYFISHUTILS_H
#define FLYFISHUTILS_H

class Vector;
class BiVector;
class TriVector;

class FlyFishUtils
{
public:

    static const BiVector xAxis;
    static const BiVector yAxis;
    static const BiVector zAxis;

    //static void DrawLine(const BiVector& line);
    //static void DrawLine(const TriVector& p1, const TriVector& p2);

    static void DrawRect(const TriVector& tv, float width, float height);
    static void DrawFillRect(const TriVector& tv, float width, float height); // tv for TriVector
};

#endif