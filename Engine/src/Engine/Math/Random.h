#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <type_traits>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/random.hpp>

class Random final
{
public:

    // ----- Generic Range -----
    template<typename T>
    static T Range(T min, T max)
    {
        static_assert(std::is_arithmetic<T>::value, "Random::Range requires a numeric type");

        if constexpr (std::is_integral<T>::value)
        {
            std::uniform_int_distribution<T> dist(min, max);
            return dist(m_Randomizer);
        }
        else
        {
            std::uniform_real_distribution<T> dist(min, max);
            return dist(m_Randomizer);
        }
    }

    // ----- Boolean -----
    static bool Bool()
    {
        std::bernoulli_distribution dist(0.5);
        return dist(m_Randomizer);
    }

    // ----- 2D/3D/4D Vectors -----
    static glm::vec2 Vec2(float min = 0.0f, float max = 1.0f)
    {
        return glm::vec2(Range(min, max), Range(min, max));
    }

    static glm::vec3 Vec3(float min = 0.0f, float max = 1.0f) {
        return glm::vec3(Range(min, max), Range(min, max), Range(min, max));
    }

    static glm::vec4 Vec4(float min = 0.0f, float max = 1.0f)
    {
        return glm::vec4(Range(min, max), Range(min, max), Range(min, max), Range(min, max));
    }

    // ----- Normalized direction vectors -----
    static glm::vec2 Direction2D()
    {
        const float angle{ Range(0.0f, glm::two_pi<float>()) };
        return glm::vec2(glm::cos(angle), glm::sin(angle));
    }

    static glm::vec3 Direction3D()
    {
        const float theta{ Range(0.0f, glm::two_pi<float>()) };
        const float phi{ Range(0.0f, glm::pi<float>()) };
        const float x{ glm::sin(phi) * glm::cos(theta) };
        const float y{ glm::sin(phi) * glm::sin(theta) };
        const float z{ glm::cos(phi) };
        return glm::vec3{ x, y, z };
    }

    // ----- Random colors (RGB) -----
    static glm::vec4 ColorRGBA()
    {
        return Vec4(0.0f, 1.f);
    }

    static glm::vec4 ColorRGBA(float alpha)
    {
        glm::vec4 color{ ColorRGBA() };
        color.a = alpha;
        return color;
	}

    static glm::vec4 ColorRGB()
    {
        glm::vec4 color{ ColorRGBA() };
        color.a = 1.f;
        return color;
    }

    // ----- Seed manually -----
    static void Seed(unsigned int seed)
    {
        m_Randomizer.seed(seed);
    }

private:

    inline static std::mt19937 m_Randomizer{ std::random_device{}() };
};

#endif // !RANDOM_H