#include "ViewportGuiLayerCNC.h"

#include <imgui.h>

ViewportGuiLayerCNC::ViewportGuiLayerCNC(MainDataLayerCNC& dataLayer)
	: BaseGuiLayerCNC(dataLayer)
{
}

void ViewportGuiLayerCNC::DrawGui()
{
	if (ImGui::Begin("Viewport ###viewport_cnc"))
	{
		auto& viewportDataLayer = m_MainDataLayer.m_ViewportDataLayer;

		auto windowPosition = ImGui::GetWindowPos();
		viewportDataLayer.m_ViewportPosition = { windowPosition.x, windowPosition.y };

		auto vMin = ImGui::GetWindowContentRegionMin();
		auto vMax = ImGui::GetWindowContentRegionMax();

		m_MainDataLayer.m_ViewportDataLayer.m_ViewportWidth = vMax.x - vMin.x;
		m_MainDataLayer.m_ViewportDataLayer.m_ViewportHeight = vMax.y - vMin.y;

		ImGui::Image(viewportDataLayer.m_ViewportImage.SRV(), { viewportDataLayer.m_ViewportWidth, viewportDataLayer.m_ViewportHeight });

		ImGui::End();
	}
}
