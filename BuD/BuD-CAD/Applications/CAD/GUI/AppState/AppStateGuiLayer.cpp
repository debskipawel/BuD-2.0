#include "AppStateGuiLayer.h"

#include <imgui.h>

#include <Applications/CAD/AppState.h>

AppStateGuiLayer::AppStateGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayerCAD(dataLayer)
{
	m_ButtonMap =
	{
		{ AppState::IDLE, { BuD::Texture::LoadFromFile("Resources/Sprites/idle_icon.png"), "Select mode (1)" } },
		{ AppState::MOVE, { BuD::Texture::LoadFromFile("Resources/Sprites/move_icon.png"), "Move mode (2)" } },
		{ AppState::ROTATE, { BuD::Texture::LoadFromFile("Resources/Sprites/rotate_icon.png"), "Rotate mode (3)" } },
		{ AppState::SCALE, { BuD::Texture::LoadFromFile("Resources/Sprites/scale_icon.png"), "Scale mode (4)" } },
	};
}

void AppStateGuiLayer::DrawGui()
{
	if (ImGui::Begin("State", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
	{
		ImVec4 normalColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		ImVec4 selectedColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

		const auto& appState = m_MainDataLayer.m_AppStateDataLayer.m_AppState;
		const auto& sceneCAD = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;

		for (auto& [state, buttonDesc] : m_ButtonMap)
		{
			auto color = appState == state ? selectedColor : normalColor;

			if (ImGui::ImageButton(buttonDesc.m_ButtonIcon.SRV(), { 32, 32 }, { 0, 0 }, { 1, 1 }, -1, color))
			{
				const auto& scene = sceneCAD.m_Scene;
				const auto& cursor = sceneCAD.m_CentroidCursor;
				
				cursor->SetAppState(state);
				m_MainDataLayer.m_AppStateDataLayer.SetAppState(state);
			}

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip(buttonDesc.m_TooltipLabel.c_str());
			}

			ImGui::SameLine();
		}

		ImGui::End();
	}
}
