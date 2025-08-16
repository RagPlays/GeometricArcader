#include "Collision.h"

#include <glm/glm.hpp>
#include "FlyFish.h"
#include "FlyFishTools/FlyFishUtils.h"

static bool HasCollision(const TriVector& pos1, const glm::vec2& size1, const TriVector& pos2, const glm::vec2& size2)
{
    const float half1x{ size1.x * 0.5f };
    const float half1y{ size1.y * 0.5f };
    const float half2x{ size2.x * 0.5f };
    const float half2y{ size2.y * 0.5f };

    return (std::abs(pos1.e032() - pos2.e032()) <= (half1x + half2x) &&
        std::abs(pos1.e013() - pos2.e013()) <= (half1y + half2y));
}

static bool HasCollisionGEOA(const TriVector& pos1, const glm::vec2& size1, const TriVector& pos2, const glm::vec2& size2)
{
    const glm::vec2 half1{ size1 * 0.5f };
    const glm::vec2 half2{ size2 * 0.5f };

    // Planes for rect1 (axis-aligned)
    std::array<Vector, 4> planes
    {
        Vector{half1.x,  1.0f, 0.0f, 0.0f},   // right
        Vector{half1.x, -1.0f, 0.0f, 0.0f},   // left
        Vector{half1.y,  0.0f, 1.0f, 0.0f},   // top
        Vector{half1.y,  0.0f,-1.0f, 0.0f}    // bottom
    };

    // Check pos2 against rect1 planes
    for (const auto& plane : planes)
    {
        const float dist{ FlyFishUtils::SignedDistanceToPlane(plane, pos2) };
        if (dist > (FlyFishUtils::IsHorizontalPlane(plane) ? half2.y : half2.x)) return false;
    }

    return true;
}