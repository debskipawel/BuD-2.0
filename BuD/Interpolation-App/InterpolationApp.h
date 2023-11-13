#pragma once

#include <BuD.h>

#include <GUI/ViewportGuiLayer.h>
#include <GUI/SimulationGuiLayer.h>
#include <GUI/KeyframeListGuiLayer.h>

#include <Objects/Frame.h>

class InterpolationApp : public BuD::AppLayer
{
public:
	InterpolationApp();

	virtual void OnUpdate(float deltaTime) override;
	virtual void OnRender() override;
	virtual void OnGuiRender() override;

	virtual void OnConcreteEvent(BuD::KeyDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseScrolledEvent& e) override;

protected:
	BuD::Scene m_EulerScene, m_QuaternionScene;
	Frame m_EulerFrame, m_QuaternionFrame;

	// ---------- DATA LAYERS ----------
	SimulationDataLayer m_DataLayer;

	// ---------- GUI LAYERS ----------
	ViewportGuiLayer m_EulerViewportGuiLayer, m_QuaternionViewportGuiLayer;
	SimulationGuiLayer m_SimulationGuiLayer;
	KeyframeListGuiLayer m_KeyframeListGuiLayer;

	float m_Time;
};
