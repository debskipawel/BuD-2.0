#include "ViewportGuiLayer.h"

#include <imgui.h>

ViewportGuiLayer::ViewportGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
}

void ViewportGuiLayer::DrawGui()
{
	DrawViewport(m_MainDataLayer.m_ViewportDataLayer1, "Viewport1");
	DrawViewport(m_MainDataLayer.m_ViewportDataLayer2, "Viewport2");
}

auto ViewportGuiLayer::DrawViewport(ViewportDataLayer& viewport, const std::string& viewportName) -> void
{
	auto label = std::format("{}###{}", viewportName, viewportName);

	if (ImGui::Begin(label.c_str()))
	{
		auto windowPosition = ImGui::GetWindowPos();
		viewport.m_ViewportPosition = { windowPosition.x, windowPosition.y };

		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += windowPosition.x;
		vMin.y += windowPosition.y;
		vMax.x += windowPosition.x;
		vMax.y += windowPosition.y;

		viewport.m_ViewportWidth = vMax.x - vMin.x;
		viewport.m_ViewportHeight = vMax.y - vMin.y;

		ImGui::Image(viewport.m_ViewportImage.SRV(), { viewport.m_ViewportWidth, viewport.m_ViewportHeight });

		ImGui::End();
	}
}
