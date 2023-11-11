#include "MainDataLayerSwitcher.h"

#include <Applications/CAD/ApplicationCAD.h>
#include <Applications/CNC/ApplicationCNC.h>

MainDataLayerSwitcher::MainDataLayerSwitcher()
{
	m_AppLayers.emplace(std::make_shared<ApplicationCAD>(), "CAD");
	m_AppLayers.emplace(std::make_shared<ApplicationCNC>(), "CNC simulator");

	m_ActiveApp = m_AppLayers.begin()->first;
}
