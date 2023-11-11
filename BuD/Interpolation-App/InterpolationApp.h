#pragma once

#include <BuD.h>

#include <GUI/ViewportGuiLayer.h>

class InterpolationApp : public BuD::AppLayer
{
public:
	InterpolationApp();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGuiRender() override;

protected:
	ViewportGuiLayer m_EulerViewportGuiLayer, m_QuaternionViewportGuiLayer;

	float m_Time;
};
