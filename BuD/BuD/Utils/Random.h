#pragma once

#include <random>

namespace BuD
{
	class Random
	{
	public:
		Random();
		Random(int seed);

		float Next();
		float Next(float min, float max);
		int Next(int min, int max);
		int Next(int max);

	private:
		std::mt19937 m_Engine;
		std::uniform_real_distribution<> m_RealDistribution;
	};
}
