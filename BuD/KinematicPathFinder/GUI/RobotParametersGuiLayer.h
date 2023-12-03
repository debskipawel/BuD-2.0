#pragma once

#include <GUI/BaseGuiLayer.h>

class RobotParametersGuiLayer : public BaseGuiLayer
{
public:
	RobotParametersGuiLayer(MainDataLayer& mainDataLayer);

	virtual auto DrawGui() -> void override;

protected:
	virtual auto DrawStartPointOptionsComboBox() -> void;
	virtual auto DrawEndPointOptionsComboBox() -> void;
	
	virtual auto DrawParameterSpaceTexture() -> void;

	BuD::Texture m_Texture;
};
