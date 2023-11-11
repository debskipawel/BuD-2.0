#include "InterpolationApp.h"

InterpolationApp::InterpolationApp()
	: m_Time(0.0f), m_EulerViewportGuiLayer("Euler"), m_QuaternionViewportGuiLayer("Quaternion")
{
}

void InterpolationApp::OnUpdate(float deltaTime)
{
	m_Time += deltaTime;
}

void InterpolationApp::OnRender()
{
	const auto& eulerViewportInfo = m_EulerViewportGuiLayer.GetViewportInfo();
	const auto& quaternionViewportInfo = m_QuaternionViewportGuiLayer.GetViewportInfo();

	auto r1 = 0.5f * sinf(m_Time) + 0.5f;
	auto g1 = 0.5f * sinf(1.15f * m_Time + 0.3f) + 0.5f;
	auto b1 = 0.5f * sinf(0.72f * m_Time - 0.1f) + 0.5f;
	auto r2 = 0.5f * sinf(m_Time) + 0.5f;
	auto g2 = 0.5f * sinf(1.39f * m_Time + 2.13f) + 0.5f;
	auto b2 = 0.5f * sinf(0.24f * m_Time - 1.38f) + 0.5f;

	BuD::Renderer::BeginTarget(eulerViewportInfo.m_Width, eulerViewportInfo.m_Height);
	BuD::Renderer::Clear(r1, g1, b1, 1.0f);

	auto eulerViewportImage = BuD::Renderer::EndTarget();

	BuD::Renderer::BeginTarget(quaternionViewportInfo.m_Width, quaternionViewportInfo.m_Height);
	BuD::Renderer::Clear(r2, g2, b2, 1.0f);

	auto quaternionViewportImage = BuD::Renderer::EndTarget();

	m_EulerViewportGuiLayer.UpdateImage(eulerViewportImage);
	m_QuaternionViewportGuiLayer.UpdateImage(quaternionViewportImage);
}

void InterpolationApp::OnGuiRender()
{
	m_EulerViewportGuiLayer.DrawGui();
	m_QuaternionViewportGuiLayer.DrawGui();
}
