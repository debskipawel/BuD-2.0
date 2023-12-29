#pragma once

#include <DataLayers/AppStateDataLayer.h>
#include <DataLayers/Scene/SceneDataLayer.h>
#include <DataLayers/SimulationDataLayer.h>
#include <DataLayers/ViewportDataLayer.h>

struct MainDataLayer
{
	MainDataLayer();

	virtual auto Update(float deltaTime) -> void;

	AppStateDataLayer m_AppStateDataLayer;
	SceneDataLayer m_SceneDataLayer;
	BaseSimulation m_SimulationDataLayer;
	ViewportDataLayer m_ViewportDataLayer;
	
	float m_AngularVelocity;
};
