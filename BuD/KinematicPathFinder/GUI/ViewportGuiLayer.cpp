#include "ViewportGuiLayer.h"

#include <imgui.h>

ViewportGuiLayer::ViewportGuiLayer(const std::string& viewportTitle, MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer), m_ViewportTitle(viewportTitle)
{
}

void ViewportGuiLayer::DrawGui()
{
	if (ImGui::Begin(m_ViewportTitle.c_str()))
	{
		auto& viewportDataLayer = m_MainDataLayer.m_ViewportDataLayer;

		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();

		auto size = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);

		viewportDataLayer.m_Width = size.x;
		viewportDataLayer.m_Height = size.y;

		ImGui::Image(viewportDataLayer.m_ViewportImage.SRV(), size);

		ImGui::End();
	}
}
