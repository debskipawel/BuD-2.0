#pragma once

#include <GUI/BaseGuiLayer.h>

class TimelineGuiLayer : public BaseGuiLayer
{
public:
	TimelineGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;

protected:
	virtual void DrawImageButton(const BuD::Texture& image, std::function<void()> onClick, dxm::Vector2 buttonSize, bool disabled = false);

	BuD::Texture m_PlayIcon, m_PauseIcon, m_LoopIcon;
};
