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

		auto windowPos = ImGui::GetWindowPos();
		auto vMin = ImGui::GetWindowContentRegionMin();
		auto vMax = ImGui::GetWindowContentRegionMax();

		viewportDataLayer.m_ViewportMin = { windowPos.x + vMin.x, windowPos.y + vMin.y };
		viewportDataLayer.m_ViewportMax = { windowPos.x + vMax.x, windowPos.y + vMax.y };

		viewportDataLayer.m_ViewportMin -= viewportDataLayer.m_WindowPos;
		viewportDataLayer.m_ViewportMax -= viewportDataLayer.m_WindowPos;

		// DEBUG: draw rect around viewport
		ImGui::GetForegroundDrawList()->AddRect({ windowPos.x + vMin.x, windowPos.y + vMin.y }, { windowPos.x + vMax.x, windowPos.y + vMax.y }, ImColor(1.0f, 0.0f, 0.0f, 1.0f));

		auto size = ImVec2(vMax.x - vMin.x, vMax.y - vMin.y);

		viewportDataLayer.m_Width = size.x;
		viewportDataLayer.m_Height = size.y;

		ImGui::Image(viewportDataLayer.m_ViewportImage.SRV(), size);

		ImGui::End();
	}
}
