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
	virtual void DrawGuiForSelectedKeyframe();

	virtual void UpdateSelectedKeyframeBasedOnTime();

	SimulationDataLayer& m_SimulationDataLayer;

	std::optional<unsigned int> m_FrameSelectedForEditing;
};
