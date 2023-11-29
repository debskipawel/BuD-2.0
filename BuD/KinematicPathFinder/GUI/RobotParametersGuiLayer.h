#pragma once

#include <BuD.h>

class RobotParametersGuiLayer
{
public:
	RobotParametersGuiLayer();

	virtual auto DrawGui() -> void;

protected:
	virtual auto DrawParameterSpaceTexture() -> void;

	BuD::Texture m_Texture;
};
