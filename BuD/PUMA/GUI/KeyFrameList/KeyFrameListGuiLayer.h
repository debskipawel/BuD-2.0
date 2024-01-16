#pragma once

#include <GUI/BaseGuiLayer.h>

#include <Robot/Animation/AnimationKeyFrame.h>

class KeyFrameListGuiLayer : public BaseGuiLayer
{
public:
	KeyFrameListGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;

protected:
	virtual auto DrawAddKeyframeButton() -> void;
	virtual auto DrawKeyframeList() -> void;
	
	virtual auto DrawGuiForSelectedKeyframe() -> void;
	virtual auto DrawTimeGuiForKeyframe(AnimationKeyFrame& keyFrame) -> void;
	virtual auto DrawConfigurationGuiForKeyframe(AnimationKeyFrame& keyFrame) -> void;
	virtual auto DrawEffectorFrameGuiForKeyframe(AnimationKeyFrame& keyFrame) -> void;
	virtual auto DrawDeleteButtonGuiForKeyframe(AnimationKeyFrame& keyFrame) -> void;

	virtual auto UpdateSelectedKeyframeBasedOnTime() -> void;

	std::optional<unsigned int> m_FrameSelectedForEditing;
};
