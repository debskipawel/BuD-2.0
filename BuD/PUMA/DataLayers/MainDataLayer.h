#pragma once

#include <BuD.h>

#include <DataLayers/Camera/CameraDataLayer.h>
#include <DataLayers/Viewport/ViewportDataLayer.h>

struct MainDataLayer
{
	MainDataLayer();

	CameraDataLayer m_CameraDataLayer;

	ViewportDataLayer m_ViewportDataLayer1;
	ViewportDataLayer m_ViewportDataLayer2;
};
