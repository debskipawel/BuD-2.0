#include "PropertiesGuiLayer.h"

#include <BuD.h>
#include <imgui.h>

#include "PerformanceGuiLayer.h"

PropertiesGuiLayer::PropertiesGuiLayer()
{
	m_PerformanceGuiLayer = std::make_unique<PerformanceGuiLayer>();
}

void PropertiesGuiLayer::DrawGui()
{
	if (ImGui::Begin("Properties"))
	{
		m_PerformanceGuiLayer->DrawGui();

		ImGui::End();
	}
}
