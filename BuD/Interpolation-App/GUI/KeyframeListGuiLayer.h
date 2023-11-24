#pragma once

#include <DataLayers/MainDataLayer.h>

class KeyframeListGuiLayer
{
public:
	KeyframeListGuiLayer(MainDataLayer& mainDataLayer);

	virtual void DrawGui();

protected:
	virtual void DrawAddKeyframeButton();
	virtual void DrawKeyframeList();
	virtual void DrawGuiForSelectedKeyframe();

	virtual void UpdateSelectedKeyframeBasedOnTime();

	MainDataLayer& m_MainDataLayer;

	std::optional<unsigned int> m_FrameSelectedForEditing;
};
