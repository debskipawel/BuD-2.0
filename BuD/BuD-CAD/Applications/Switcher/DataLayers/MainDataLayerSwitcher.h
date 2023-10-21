#pragma once

#include <BuD.h>

struct MainDataLayerSwitcher
{
	MainDataLayerSwitcher();
	
	std::map<std::shared_ptr<BuD::AppLayer>, std::string> m_AppLayers;
	
	std::shared_ptr<BuD::AppLayer> m_ActiveApp;
};
