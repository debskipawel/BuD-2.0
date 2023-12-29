#pragma once

#include <GUI/BaseGuiLayer.h>

class KeyFrameListGuiLayer : public BaseGuiLayer
{
public:
	KeyFrameListGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;

protected:
	virtual auto DrawAddKeyframeButton() -> void;
	virtual auto DrawKeyframeList() -> void;
	virtual auto DrawGuiForSelectedKeyframe() -> void;

	virtual auto UpdateSelectedKeyframeBasedOnTime() -> void;

	std::optional<unsigned int> m_FrameSelectedForEditing;
};
