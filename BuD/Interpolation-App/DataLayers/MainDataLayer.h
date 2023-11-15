#pragma once

#include <DataLayers/SceneDataLayer.h>
#include <DataLayers/SimulationDataLayer.h>

struct MainDataLayer
{
	MainDataLayer();

	virtual void UpdateGhostMesh();

	SceneDataLayer m_SceneDataLayer;
	SimulationDataLayer m_SimulationDataLayer;

	bool m_ShowGhost;
	unsigned int m_IntermediateFramesCount;
};
