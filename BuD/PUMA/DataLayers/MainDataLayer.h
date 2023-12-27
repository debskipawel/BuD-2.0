#pragma once

#include <BuD.h>

#include <DataLayers/Camera/CameraDataLayer.h>
#include <DataLayers/Simulation/SimulationDataLayer.h>

struct MainDataLayer
{
	MainDataLayer();

	CameraDataLayer m_CameraDataLayer;

	std::vector<std::shared_ptr<SimulationDataLayer>> m_Simulations;
};
