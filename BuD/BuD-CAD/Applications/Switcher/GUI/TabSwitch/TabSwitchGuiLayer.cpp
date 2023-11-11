#include "TabSwitchGuiLayer.h"

#include <imgui.h>

TabSwitchGuiLayer::TabSwitchGuiLayer(MainDataLayerSwitcher& dataLayer)
	: BaseGuiLayerSwitcher(dataLayer)
{
}

void TabSwitchGuiLayer::DrawGui()
{
	const ImVec4 colorNormal = ImVec4(0.0f, 0.2f, 0.5f, 1.0f);
	const ImVec4 colorPressed = ImVec4(0.0f, 0.1f, 0.3f, 1.0f);

	if (ImGui::Begin("###tab_bar", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
	{
		for (auto& [appLayer, name] : m_MainDataLayer.m_AppLayers)
		{
			auto pressed = appLayer == m_MainDataLayer.m_ActiveApp;

			auto tag = std::format("{}##tab_{}", name, name);

			ImGui::PushStyleColor(ImGuiCol_Button, pressed ? colorPressed : colorNormal);

			if (ImGui::Button(tag.c_str()))
			{
				m_MainDataLayer.m_ActiveApp = appLayer;
			}

			ImGui::PopStyleColor();

			ImGui::SameLine();
		}

		ImGui::End();
	}
}
