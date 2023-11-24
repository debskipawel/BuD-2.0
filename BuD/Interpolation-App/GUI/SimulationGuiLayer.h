#pragma once

#include <BuD.h>

#include <DataLayers/MainDataLayer.h>
#include <DataLayers/SimulationDataLayer.h>

class SimulationGuiLayer
{
public:
	SimulationGuiLayer(MainDataLayer& dataLayer);
	virtual void DrawGui();
	
protected:
	virtual void DrawSimulationSettingsGui();
	virtual void DrawSimulationTimelineGui();

	virtual void DrawImageButton(const BuD::Texture& image, std::function<void()> onClick, dxm::Vector2 buttonSize, bool disabled = false);

	MainDataLayer& m_MainDataLayer;

private:
	BuD::Texture m_PlayIcon, m_PauseIcon, m_LoopIcon;
};
