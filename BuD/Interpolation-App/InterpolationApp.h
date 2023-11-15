#pragma once

#include <BuD.h>

#include <DataLayers/MainDataLayer.h>

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
	virtual void OnConcreteEvent(BuD::MouseButtonDownEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseButtonReleasedEvent& e) override;
	virtual void OnConcreteEvent(BuD::MouseMovedEvent& e) override;

protected:
	bool m_MouseMove;

	// ---------- DATA LAYERS ----------
	MainDataLayer m_MainDataLayer;

	// ---------- GUI LAYERS ----------
	ViewportGuiLayer m_EulerViewportGuiLayer, m_QuaternionViewportGuiLayer;
	SimulationGuiLayer m_SimulationGuiLayer;
	KeyframeListGuiLayer m_KeyframeListGuiLayer;

	float m_Time;
};
