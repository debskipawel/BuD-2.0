#pragma once

#include <BuD.h>

#include <GUI/AppStateGuiLayer.h>
#include <GUI/RobotParametersGuiLayer.h>
#include <GUI/ViewportGuiLayer.h>

class KinematicPathFinderApp : public BuD::AppLayer
{
public:
	KinematicPathFinderApp();

	virtual auto OnUpdate(float deltaTime) -> void override;
	virtual auto OnGuiRender() -> void override;
	virtual auto OnRender() -> void override;

protected:
	AppStateGuiLayer m_AppStateGui;
	RobotParametersGuiLayer m_RobotParametersGui;
	ViewportGuiLayer m_ViewportGui;
};
