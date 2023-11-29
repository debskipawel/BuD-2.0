#include "ViewportGuiLayer.h"

ViewportGuiLayer::ViewportGuiLayer(const std::string& viewportTitle)
	: m_ViewportTitle(viewportTitle), m_ViewportInfo()
{
}

void ViewportGuiLayer::DrawGui()
{
	if (ImGui::Begin(m_ViewportTitle.c_str()))
	{
		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		auto size = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);

		m_ViewportInfo.m_Width = size.x;
		m_ViewportInfo.m_Height = size.y;

		ImGui::Image(m_ViewportImage.SRV(), size);

		ImGui::End();
	}
}

void ViewportGuiLayer::UpdateImage(const BuD::Texture& texture)
{
	m_ViewportImage = texture;
}
