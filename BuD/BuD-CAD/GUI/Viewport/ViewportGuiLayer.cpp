#include "ViewportGuiLayer.h"

#include <imgui.h>

ViewportGuiLayer::ViewportGuiLayer(ViewportViewModel& viewModel)
	: m_ViewModel(viewModel)
{
}

void ViewportGuiLayer::DrawGui()
{
	if (ImGui::Begin("Viewport"))
	{
		auto windowPosition = ImGui::GetWindowPos();
		m_ViewModel.m_ViewportPosition = { windowPosition.x, windowPosition.y };

		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += windowPosition.x;
		vMin.y += windowPosition.y;
		vMax.x += windowPosition.x;
		vMax.y += windowPosition.y;

		m_ViewModel.m_ViewportWidth = vMax.x - vMin.x;
		m_ViewModel.m_ViewportHeight = vMax.y - vMin.y;

		ImGui::Image(m_ViewModel.m_ViewportImage.SRV(), { m_ViewModel.m_ViewportWidth, m_ViewModel.m_ViewportHeight });

		ImGui::End();
	}
}
