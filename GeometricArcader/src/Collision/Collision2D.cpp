#include "Collision2D.h"

#include <glm/glm.hpp>

#include "FlyFish.h"
#include "FlyFishTools/FlyFishUtils.h"

bool Collision2D::HasCollision(const TriVector& point0, const glm::vec2& size0, const TriVector& point1, const glm::vec2& size1)
{
    // Standard (AABB) overlapping check

    const glm::vec2 pos0{ FlyFishUtils::ToVec2(point0) };
    const glm::vec2 pos1{ FlyFishUtils::ToVec2(point1) };
    const glm::vec2 half0{ size0 * 0.5f };
    const glm::vec2 half1{ size1 * 0.5f };

    const glm::vec2 diff{ glm::abs(pos0 - pos1) };

	return diff.x <= (half0.x + half1.x) && diff.y <= (half0.y + half1.y);
}

// Made with ChatGPT //
bool Collision2D::HasCollisionGEOA(const TriVector& pos1, const glm::vec2& size1, const TriVector& pos2, const glm::vec2& size2)
{
    constexpr float epsilon{ 1e-6f };

    auto makeCorners = [](const TriVector& center, const glm::vec2& size)
    {
        glm::vec2 c = FlyFishUtils::ToVec2(center);
        glm::vec2 h = size * 0.5f;
        return std::array<TriVector, 4>
        {
                TriVector(c.x - h.x, c.y - h.y, 0.0f), // 0: bl
                TriVector(c.x + h.x, c.y - h.y, 0.0f), // 1: br
                TriVector(c.x + h.x, c.y + h.y, 0.0f), // 2: tr
                TriVector(c.x - h.x, c.y + h.y, 0.0f)  // 3: tl
        };
    };

    auto A = makeCorners(pos1, size1);
    auto B = makeCorners(pos2, size2);

    // 2D point-in-convex-quad test using cross product (keeps orientation-agnostic)
    auto pointInRect2D = [](const TriVector& p, const std::array<TriVector, 4>& R) -> bool
    {
        glm::vec2 pt = FlyFishUtils::ToVec2(p);
        for (int i = 0; i < 4; ++i) {
            glm::vec2 a = FlyFishUtils::ToVec2(R[i]);
            glm::vec2 b = FlyFishUtils::ToVec2(R[(i + 1) % 4]);
            glm::vec2 edge = b - a;
            glm::vec2 toP = pt - a;
            float cross = edge.x * toP.y - edge.y * toP.x;
            if (cross < 0.0f) return false; // point is outside for this edge
        }
        return true;
    };

    // Quick containment checks (corner inside other rect)
    if (pointInRect2D(A[0], B) || pointInRect2D(A[1], B) ||
        pointInRect2D(A[2], B) || pointInRect2D(A[3], B))
    {
        return true;
    }

    if (pointInRect2D(B[0], A) || pointInRect2D(B[1], A) ||
        pointInRect2D(B[2], A) || pointInRect2D(B[3], A))
        return true;

    // Helper to check if 2D point is between two endpoints (inclusive)
    auto betweenInclusive = [](const glm::vec2& p, const glm::vec2& a, const glm::vec2& b) -> bool
    {
        glm::vec2 mn = glm::min(a, b);
        glm::vec2 mx = glm::max(a, b);
        return p.x + epsilon >= mn.x && p.x - epsilon <= mx.x &&
            p.y + epsilon >= mn.y && p.y - epsilon <= mx.y;
    };

    // Edge vs edge intersection using PPGA meet
    for (int i = 0; i < 4; ++i)
    {
        TriVector a0 = A[i];
        TriVector a1 = A[(i + 1) % 4];
        BiVector la = a0 & a1; // line of edge A

        for (int j = 0; j < 4; ++j)
        {
            TriVector b0 = B[j];
            TriVector b1 = B[(j + 1) % 4];
            BiVector lb = b0 & b1; // line of edge B

            TriVector meet = (la ^ lb).Grade3(); // meet point (if finite)
            if (std::fabs(meet.e123()) < epsilon)
            {
                // parallel or at infinity
                continue;
            }

            glm::vec2 m = FlyFishUtils::ToVec2(meet);
            glm::vec2 aa = FlyFishUtils::ToVec2(a0);
            glm::vec2 ab = FlyFishUtils::ToVec2(a1);
            glm::vec2 ba = FlyFishUtils::ToVec2(b0);
            glm::vec2 bb = FlyFishUtils::ToVec2(b1);

            if (betweenInclusive(m, aa, ab) && betweenInclusive(m, ba, bb)) return true;
        }
    }

    return false;
}