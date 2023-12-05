#include "AppStateGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

AppStateGuiLayer::AppStateGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
	m_ButtonMap.emplace(AppState::EDIT_START, AppStateButton{ BuD::Texture::LoadFromFile("Resources/Sprites/start-edit.png"), "Edit Start Configuration" });
	m_ButtonMap.emplace(AppState::EDIT_END, AppStateButton{ BuD::Texture::LoadFromFile("Resources/Sprites/end-edit.png"), "Edit End Configuration" });
	m_ButtonMap.emplace(AppState::ADD_OBSTACLE, AppStateButton{ BuD::Texture::LoadFromFile("Resources/Sprites/obstacle-edit.png"), "Add Obstacles" });
	m_ButtonMap.emplace(
		AppState::ANIMATION_RUNNING, 
		AppStateButton
		{ 
			BuD::Texture::LoadFromFile("Resources/Sprites/play_icon.png"), 
			"Play Animation", 
			[this]() { return RunSimulation(); } 
		}
	);
}

auto AppStateGuiLayer::DrawGui() -> void
{
	if (ImGui::Begin("AppState"))
	{
		ImVec4 selectedColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		ImVec4 normalColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

		const auto& appState = m_MainDataLayer.m_AppStateDataLayer.m_AppState;

		for (auto& [state, buttonDesc] : m_ButtonMap)
		{
			auto selected = appState == state;
			auto disabled = selected || (appState == AppState::ANIMATION_RUNNING);

			ImGui::PushStyleColor(ImGuiCol_Button, selected ? selectedColor : normalColor);
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disabled);

			if (ImGui::ImageButton(buttonDesc.m_ButtonIcon.SRV(), { 32, 32 }, { 0, 0 }, { 1, 1 }, -1))
			{
				if (buttonDesc.m_OnClick())
				{
					m_MainDataLayer.m_AppStateDataLayer.m_AppState = state;
				}
			}

			ImGui::PopStyleColor();
			ImGui::PopItemFlag();

			if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
			{
				ImGui::SetTooltip(buttonDesc.m_TooltipLabel.c_str());
			}

			ImGui::SameLine();
		}

		ImGui::End();
	}
}

auto AppStateGuiLayer::RunSimulation() -> bool
{
	auto& sceneDataLayer = m_MainDataLayer.m_SceneDataLayer;

	if (!sceneDataLayer.m_StartConfiguration.Valid() || !sceneDataLayer.m_EndConfiguration.Valid())
	{
		return false;
	}

	auto path = sceneDataLayer.FindPathFromStartingConfiguration();

	m_MainDataLayer.m_SimulationDataLayer.StartSimulation(path);

	return true;
}
