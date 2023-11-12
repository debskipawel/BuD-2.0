#pragma once

#include <BuD.h>

#include <GUI/ViewportGuiLayer.h>
#include <GUI/SimulationSettingsGuiLayer.h>

#include <Objects/Frame.h>

class InterpolationApp : public BuD::AppLayer
{
public:
	InterpolationApp();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGuiRender() override;

protected:
	BuD::Scene m_EulerScene, m_QuaternionScene;
	Frame m_EulerFrame, m_QuaternionFrame;

	// ---------- GUI LAYERS -----------
	ViewportGuiLayer m_EulerViewportGuiLayer, m_QuaternionViewportGuiLayer;
	SimulationSettingsGuiLayer m_SimulationGuiLayer;

	float m_Time;
};
