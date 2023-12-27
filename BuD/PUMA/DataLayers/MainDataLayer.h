#pragma once

#include <BuD.h>

#include <DataLayers/Camera/CameraDataLayer.h>
#include <DataLayers/Viewport/ViewportDataLayer.h>

struct MainDataLayer
{
	MainDataLayer(BuD::Scene& scene);

	CameraDataLayer m_CameraDataLayer;
	ViewportDataLayer m_ViewportDataLayer;

	BuD::Scene& m_Scene;
};
