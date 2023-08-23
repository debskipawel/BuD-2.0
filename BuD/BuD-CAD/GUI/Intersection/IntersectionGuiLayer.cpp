#include "IntersectionGuiLayer.h"

#include <imgui.h>

IntersectionGuiLayer::IntersectionGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
}

void IntersectionGuiLayer::DrawGui()
{
	if (!m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected.EligibleForIntersection())
	{
		return;
	}

	if (ImGui::Begin("Intersection"))
	{
		ImGui::End();
	}
}
