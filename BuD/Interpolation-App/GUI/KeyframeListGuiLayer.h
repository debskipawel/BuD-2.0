#pragma once

#include <DataLayers/SimulationDataLayer.h>

class KeyframeListGuiLayer
{
public:
	KeyframeListGuiLayer(SimulationDataLayer& simulationDataLayer);

	virtual void DrawGui();

protected:
	virtual void DrawAddKeyframeButton();
	virtual void DrawKeyframeList();

	SimulationDataLayer& m_SimulationDataLayer;

	int m_FrameSelectedForEditing;
	bool m_FrameSelectedManually;
};
