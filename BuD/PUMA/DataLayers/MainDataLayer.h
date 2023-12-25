#pragma once

#include <BuD.h>

#include <DataLayers/ViewportDataLayer.h>

struct MainDataLayer
{
	MainDataLayer(BuD::Scene& scene);

	ViewportDataLayer m_ViewportDataLayer;

	BuD::Scene& m_Scene;
};
