#pragma once

#include <BuD.h>

#include <DataLayers/SimulationDataLayer.h>

class SimulationGuiLayer
{
public:
	SimulationGuiLayer(SimulationDataLayer& dataLayer);
	virtual void DrawGui();
	
protected:
	virtual void DrawSimulationSettingsGui();
	virtual void DrawSimulationTimelineGui();

	virtual void DrawImageButton(const BuD::Texture& image, std::function<void()> onClick, dxm::Vector2 buttonSize, bool disabled = false);

	SimulationDataLayer& m_SimulationDataLayer;

private:
	BuD::Texture m_PlayIcon, m_PauseIcon, m_LoopIcon;
};
