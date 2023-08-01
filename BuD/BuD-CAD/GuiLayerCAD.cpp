#include "GuiLayerCAD.h"

#include <imgui.h>

GuiLayerCAD::GuiLayerCAD()
{
}

void GuiLayerCAD::DrawGui(MainViewModel& viewModel)
{
	if (ImGui::Begin("Viewport"))
	{
		auto& viewportVM = viewModel.m_ViewportViewModel;

		ImVec2 vMin = ImGui::GetWindowContentRegionMin();
		ImVec2 vMax = ImGui::GetWindowContentRegionMax();
		vMin.x += ImGui::GetWindowPos().x;
		vMin.y += ImGui::GetWindowPos().y;
		vMax.x += ImGui::GetWindowPos().x;
		vMax.y += ImGui::GetWindowPos().y;

		viewportVM.m_ViewportWidth = vMax.x - vMin.x;
		viewportVM.m_ViewportHeight = vMax.y - vMin.y;

		ImGui::Image(viewportVM.m_ViewportImage.SRV(), { viewportVM.m_ViewportWidth, viewportVM.m_ViewportHeight });

		ImGui::End();
	}

	if (ImGui::Begin("Object list"))
	{
		ImGui::End();
	}

	if (ImGui::Begin("Properties"))
	{
		if (ImGui::Begin("Performance"))
		{
			ImGui::End();
		}

		ImGui::End();
	}

	if (ImGui::Begin("Console"))
	{
		ImGui::End();
	}
}
