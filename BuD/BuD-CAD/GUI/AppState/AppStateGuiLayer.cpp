#include "AppStateGuiLayer.h"

#include <imgui.h>

#include <AppState.h>

AppStateGuiLayer::AppStateGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
	m_IdleIcon = BuD::Texture::LoadFromFile("Resources/Sprites/idle_icon.png");
	m_MoveIcon = BuD::Texture::LoadFromFile("Resources/Sprites/move_icon.png");
	m_RotateIcon = BuD::Texture::LoadFromFile("Resources/Sprites/rotate_icon.png");
	m_ScaleIcon = BuD::Texture::LoadFromFile("Resources/Sprites/scale_icon.png");
}

void AppStateGuiLayer::DrawGui()
{
	if (ImGui::Begin("State", nullptr, ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoResize))
	{
		ImVec4 normalColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		ImVec4 selectedColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

		std::map<AppState, BuD::Texture> iconMap = {
			{ AppState::IDLE, m_IdleIcon },
			{ AppState::MOVE, m_MoveIcon },
			{ AppState::ROTATE, m_RotateIcon },
			{ AppState::SCALE, m_ScaleIcon },
		};

		const auto& appState = m_MainDataLayer.m_AppStateDataLayer.m_AppState;
		const auto& sceneCAD = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD;

		for (auto& [state, icon] : iconMap)
		{
			auto color = appState == state ? selectedColor : normalColor;

			if (ImGui::ImageButton(icon.SRV(), { 32, 32 }, { 0, 0 }, { 1, 1 }, -1, color))
			{
				const auto& scene = sceneCAD.m_Scene;
				const auto& cursor = sceneCAD.m_MainCursor;
				
				cursor->SetAppState(state);
				m_MainDataLayer.m_AppStateDataLayer.SetAppState(state);
			}

			ImGui::SameLine();
		}

		ImGui::End();
	}
}
