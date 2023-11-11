#include "InterpolationApp.h"

#include <imgui.h>

InterpolationApp::InterpolationApp()
	: m_Time(0.0f), m_ViewportSize1(), m_ViewportSize2()
{
}

void InterpolationApp::OnUpdate(float deltaTime)
{
	m_Time += deltaTime;
}

void InterpolationApp::OnRender()
{
	auto r1 = 0.5f * sinf(m_Time) + 0.5f;
	auto g1 = 0.5f * sinf(1.15f * m_Time + 0.3f) + 0.5f;
	auto b1 = 0.5f * sinf(0.72f * m_Time - 0.1f) + 0.5f;

	BuD::Renderer::BeginTarget(m_ViewportSize1.x, m_ViewportSize1.y);
	BuD::Renderer::Clear(r1, g1, b1, 1.0f);

	m_Viewport1 = BuD::Renderer::EndTarget();

	auto r2 = 0.5f * sinf(m_Time) + 0.5f;
	auto g2 = 0.5f * sinf(1.39f * m_Time + 2.13f) + 0.5f;
	auto b2 = 0.5f * sinf(0.24f * m_Time - 1.38f) + 0.5f;

	BuD::Renderer::BeginTarget(m_ViewportSize2.x, m_ViewportSize2.y);
	BuD::Renderer::Clear(r2, g2, b2, 1.0f);

	m_Viewport2 = BuD::Renderer::EndTarget();
}

void InterpolationApp::OnGuiRender()
{
	if (ImGui::Begin("Viewport 1"))
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		auto size = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);
		m_ViewportSize1 = { size.x, size.y };

		ImGui::Image(m_Viewport1.SRV(), size);

		ImGui::End();
	}

	if (ImGui::Begin("Viewport 2"))
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		auto size = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);
		m_ViewportSize2 = { size.x, size.y };

		ImGui::Image(m_Viewport2.SRV(), size);

		ImGui::End();
	}
}
