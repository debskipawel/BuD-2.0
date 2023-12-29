#pragma once

#include <BuD.h>

struct BaseSimulation
{
public:
	BaseSimulation();

	virtual auto StartSimulation(std::vector<std::pair<int, int>> path) -> void;
	virtual auto Update(float deltaTime) -> void;

	virtual auto IsFinished() -> bool;

	virtual auto GetConfiguration() -> std::pair<float, float>;

	float m_AngularVelocity;
	
protected:
	template<typename T, typename U>
	auto GetShortestAngleDifference(std::pair<T, T> p0, std::pair<U, U> p1) -> auto
	{
		auto difference = std::make_pair(p1.first - p0.first, p1.second - p0.second);

		if (abs(difference.first) > 180)
		{
			difference.first = (difference.first > 0) ? (difference.first - 360) : (difference.first + 360);
		}

		if (abs(difference.second) > 180)
		{
			difference.second = (difference.second > 0) ? (difference.second - 360) : (difference.second + 360);
		}

		return difference;
	}

	std::vector<std::pair<int, int>> m_Path;
	std::pair<float, float> m_CurrentConfiguration;

	size_t m_CurrentIndex;
};
