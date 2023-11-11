#pragma once

#include <BuD.h>

#include <GUI/ViewportGuiLayer.h>
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

	ViewportGuiLayer m_EulerViewportGuiLayer, m_QuaternionViewportGuiLayer;

	float m_Time;
};
