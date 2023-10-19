#include "ViewportGuiLayer.h"

#include <imgui.h>

ViewportGuiLayer::ViewportGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayerCAD(dataLayer)
{
}

void ViewportGuiLayer::DrawGui()
{
	if (ImGui::Begin("Viewport"))
	{
		auto& viewportDataLayer = m_MainDataLayer.m_ViewportDataLayer;

		auto windowPosition = ImGui::GetWindowPos();
		viewportDataLayer.m_ViewportPosition = { windowPosition.x, windowPosition.y };

		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += windowPosition.x;
		vMin.y += windowPosition.y;
		vMax.x += windowPosition.x;
		vMax.y += windowPosition.y;

		m_MainDataLayer.m_ViewportDataLayer.m_ViewportWidth = vMax.x - vMin.x;
		m_MainDataLayer.m_ViewportDataLayer.m_ViewportHeight = vMax.y - vMin.y;

		ImGui::Image(viewportDataLayer.m_ViewportImage.SRV(), { viewportDataLayer.m_ViewportWidth, viewportDataLayer.m_ViewportHeight });

		ImGui::End();
	}
}
