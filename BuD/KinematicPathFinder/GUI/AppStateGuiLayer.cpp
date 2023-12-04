#include "AppStateGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

AppStateGuiLayer::AppStateGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
	m_ButtonMap.emplace(AppState::EDIT_START, AppStateButton{ BuD::Texture::LoadFromFile("Resources/Sprites/start-edit.png"), "Edit Start Configuration" });
	m_ButtonMap.emplace(AppState::EDIT_END, AppStateButton{ BuD::Texture::LoadFromFile("Resources/Sprites/end-edit.png"), "Edit End Configuration" });
	m_ButtonMap.emplace(AppState::ADD_OBSTACLE, AppStateButton{ BuD::Texture::LoadFromFile("Resources/Sprites/obstacle-edit.png"), "Add Obstacles" });
	m_ButtonMap.emplace(AppState::ANIMATION_RUNNING, AppStateButton{ BuD::Texture::LoadFromFile("Resources/Sprites/play_icon.png"), "Run Animation", [this]() { StartAnimation(); } });
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

			ImGui::PushStyleColor(ImGuiCol_Button, selected ? selectedColor : normalColor);
			ImGui::PushItemFlag(ImGuiItemFlags_Disabled, selected);

			if (ImGui::ImageButton(buttonDesc.m_ButtonIcon.SRV(), { 32, 32 }, { 0, 0 }, { 1, 1 }, -1))
			{
				m_MainDataLayer.m_AppStateDataLayer.m_AppState = state;

				buttonDesc.m_OnClick();
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

auto AppStateGuiLayer::StartAnimation() -> void
{
	auto path = m_MainDataLayer.m_SceneDataLayer.FindPathFromStartingConfiguration();
}
