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
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		m_ViewModel.m_ViewportWidth = vMax.x - vMin.x;
		m_ViewModel.m_ViewportHeight = vMax.y - vMin.y;

		ImGui::Image(m_ViewModel.m_ViewportImage.SRV(), { m_ViewModel.m_ViewportWidth, m_ViewModel.m_ViewportHeight });

		ImGui::End();
	}
}
