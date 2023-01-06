#include "bud_pch.h"
#include "Random.h"

namespace BuD
{
    Random::Random()
    {
        std::random_device rd;
        m_Engine = std::mt19937(rd());

        m_RealDistribution = std::uniform_real_distribution<>(0.0f, 1.0f);
    }

    Random::Random(int seed)
    {
        m_Engine = std::mt19937(seed);
        m_RealDistribution = std::uniform_real_distribution<>(0.0f, 1.0f);
    }

    float Random::Next()
    {
        return static_cast<float>(m_RealDistribution(m_Engine));
    }

    float Random::Next(float min, float max)
    {
        if (min > max)
        {
            return 0.0f;
        }

        return Next() * (max - min) + min;
    }
    
    int Random::Next(int min, int max)
    {
        if (max <= min)
        {
            return min;
        }

        auto intDistribution = std::uniform_int_distribution<>(min, max);

        return intDistribution(m_Engine);
    }
    
    int Random::Next(int max)
    {
        return Next(0, max);
    }
}
