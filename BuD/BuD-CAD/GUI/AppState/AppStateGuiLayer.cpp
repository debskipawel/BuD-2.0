#include "AppStateGuiLayer.h"

#include <imgui.h>

#include <AppState.h>

AppStateGuiLayer::AppStateGuiLayer(AppStateViewModel& viewModel)
	: m_ViewModel(viewModel)
{
	m_IdleIcon = BuD::Texture::LoadFromFile("Resources/Sprites/idle_icon.png");
	m_MoveIcon = BuD::Texture::LoadFromFile("Resources/Sprites/move_icon.png");
	m_RotateIcon = BuD::Texture::LoadFromFile("Resources/Sprites/rotate_icon.png");
	m_ScaleIcon = BuD::Texture::LoadFromFile("Resources/Sprites/scale_icon.png");
}

void AppStateGuiLayer::DrawGui()
{
	if (ImGui::Begin("State"))
	{
		ImVec4 normalColor = ImVec4(0.4f, 0.4f, 0.4f, 1.0f);
		ImVec4 selectedColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

		std::map<AppState, BuD::Texture> iconMap = {
			{ AppState::IDLE, m_IdleIcon },
			{ AppState::MOVE, m_MoveIcon },
			{ AppState::ROTATE, m_RotateIcon },
			{ AppState::SCALE, m_ScaleIcon },
		};

		for (auto& [state, icon] : iconMap)
		{
			auto color = m_ViewModel.m_AppState == state ? selectedColor : normalColor;

			if (ImGui::ImageButton(icon.SRV(), { 32, 32 }, { 0, 0 }, { 1, 1 }, -1, color))
			{
				m_ViewModel.m_AppState = state;
			}

			ImGui::SameLine();
		}

		ImGui::End();
	}
}
