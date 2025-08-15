#include "Random.h"

#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

namespace Engine
{
    std::mt19937 Random::m_Randomizer{ std::random_device{}() };

    // ----- Boolean -----
    bool Random::Bool()
    {
        std::bernoulli_distribution dist(0.5);
        return dist(m_Randomizer);
    }

    // ----- 2D/3D/4D Vectors -----
    glm::vec2 Random::Vec2(float min, float max)
    {
        return glm::vec2(Range(min, max), Range(min, max));
    }

    glm::vec3 Random::Vec3(float min, float max)
    {
        return glm::vec3(Range(min, max), Range(min, max), Range(min, max));
    }

    glm::vec4 Random::Vec4(float min, float max)
    {
        return glm::vec4(Range(min, max), Range(min, max), Range(min, max), Range(min, max));
    }

    // ----- Normalized direction vectors -----
    glm::vec2 Random::Direction2D()
    {
        const float angle{ Range(0.0f, glm::two_pi<float>()) };
        return glm::vec2(glm::cos(angle), glm::sin(angle));
    }

    glm::vec3 Random::Direction3D()
    {
        const float theta{ Range(0.0f, glm::two_pi<float>()) };
        const float phi{ Range(0.0f, glm::pi<float>()) };
        const float x{ glm::sin(phi) * glm::cos(theta) };
        const float y{ glm::sin(phi) * glm::sin(theta) };
        const float z{ glm::cos(phi) };
        return glm::vec3{ x, y, z };
    }

    // ----- Random colors (RGB) -----
    glm::vec4 Random::ColorRGBA()
    {
        return Vec4(0.0f, 1.f);
    }

    glm::vec4 Random::ColorRGBA(float alpha)
    {
        glm::vec4 color{ ColorRGBA() };
        color.a = alpha;
        return color;
    }

    glm::vec4 Random::ColorRGB()
    {
        glm::vec4 color{ ColorRGBA() };
        color.a = 1.f;
        return color;
    }

    // ----- Seed manually -----
    void Random::Seed(unsigned int seed)
    {
        m_Randomizer.seed(seed);
    }
}