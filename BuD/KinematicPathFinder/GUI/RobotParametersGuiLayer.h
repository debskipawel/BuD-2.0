#pragma once

#include <GUI/BaseGuiLayer.h>

class RobotParametersGuiLayer : public BaseGuiLayer
{
public:
	RobotParametersGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;

protected:
	virtual auto DrawParameterSpaceTexture() -> void;

	BuD::Texture m_Texture;
};
