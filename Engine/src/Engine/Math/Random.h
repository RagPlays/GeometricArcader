#ifndef RANDOM_H
#define RANDOM_H

#include <random>
#include <type_traits>

#include <glm/fwd.hpp> // forward declarations

namespace Engine
{
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
                return dist(s_Randomizer);
            }
            else
            {
                std::uniform_real_distribution<T> dist(min, max);
                return dist(s_Randomizer);
            }
        }

        // ----- Boolean -----
        static bool Bool();

        // ----- 2D/3D/4D Vectors -----
        static glm::vec2 Vec2(float min = 0.0f, float max = 1.0f);
        static glm::vec3 Vec3(float min = 0.0f, float max = 1.0f);
        static glm::vec4 Vec4(float min = 0.0f, float max = 1.0f);

        // ----- Normalized direction vectors -----
        static glm::vec2 Direction2D();
        static glm::vec3 Direction3D();

        // ----- Random colors (RGB) -----
        static glm::vec4 ColorRGBA();
        static glm::vec4 ColorRGBA(float alpha);
        static glm::vec4 ColorRGB();

        // ----- Seed manually -----
        static void Seed(unsigned int seed);

    private:

        static std::mt19937 s_Randomizer;
    };
}

#endif // !RANDOM_H