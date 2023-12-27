#include "MainDataLayer.h"

MainDataLayer::MainDataLayer()
	: m_Simulations()
{
	m_Simulations.emplace_back(std::make_shared<SimulationDataLayer>());
	m_Simulations.emplace_back(std::make_shared<SimulationDataLayer>());
}
