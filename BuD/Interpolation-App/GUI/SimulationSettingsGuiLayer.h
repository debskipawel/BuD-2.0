#pragma once

#include <BuD.h>

class SimulationSettingsGuiLayer
{
public:
	SimulationSettingsGuiLayer();

	virtual void DrawGui();
	
protected:
	virtual void DrawSimulationSettingsGui();
	virtual void DrawSimulationTimelineGui();

	int m_KeyFramesCount;
	float m_SimulationTime;

	BuD::Texture m_PlayIcon, m_PauseIcon;
};
