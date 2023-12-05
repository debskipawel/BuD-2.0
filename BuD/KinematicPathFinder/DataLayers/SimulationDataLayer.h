#pragma once

#include <BuD.h>

struct SimulationDataLayer
{
public:
	SimulationDataLayer();

	virtual auto StartSimulation(std::vector<std::pair<int, int>> path) -> void;
	virtual auto Update(float deltaTime) -> void;

	virtual auto IsFinished() -> bool;

	virtual auto GetConfiguration() -> std::pair<float, float>;

	float m_AngularVelocity;
	
protected:
	std::vector<std::pair<int, int>> m_Path;
	std::pair<float, float> m_CurrentConfiguration;

	size_t m_CurrentIndex;
};
