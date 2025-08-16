#ifndef COLLISION_H
#define COLLISION_H

#include <glm/fwd.hpp> // Forwards

class TriVector;

class Collision final
{
public:

    static bool HasCollision(const TriVector& pos1, const glm::vec2& size1, const TriVector& pos2, const glm::vec2& size2);
    static bool HasCollisionGEOA(const TriVector& pos1, const glm::vec2& size1, const TriVector& pos2, const glm::vec2& size2);
};

#endif // !COLLISION_H
