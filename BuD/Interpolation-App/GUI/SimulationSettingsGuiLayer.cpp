#include "SimulationSettingsGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

SimulationSettingsGuiLayer::SimulationSettingsGuiLayer()
	: m_KeyFramesCount(10), m_SimulationTime(5.0f)
{
	m_PlayIcon = BuD::Texture::LoadFromFile("Resources/Sprites/play_icon.png");
	m_PauseIcon = BuD::Texture::LoadFromFile("Resources/Sprites/pause_icon.png");
}

void SimulationSettingsGuiLayer::DrawGui()
{
	DrawSimulationSettingsGui();
	DrawSimulationTimelineGui();
}

void SimulationSettingsGuiLayer::DrawSimulationSettingsGui()
{
	if (ImGui::Begin("Simulation settings"))
	{
		ImGui::Text("Key frames count");
		ImGui::DragInt("###key_frames_count", &m_KeyFramesCount, 1.0f, 0, 1e5, "%d", ImGuiSliderFlags_AlwaysClamp);

		ImGui::Text("Time duration");
		ImGui::DragFloat("###animation_duration", &m_SimulationTime, 0.1f, 0.0f, 1000.0f, "%.1f s", ImGuiSliderFlags_AlwaysClamp);

		ImGui::End();
	}
}

void SimulationSettingsGuiLayer::DrawSimulationTimelineGui()
{
	if (ImGui::Begin("Timeline"))
	{
		auto buttonSize = ImVec2(20.0f, 20.0f);

		if (ImGui::ImageButton(m_PlayIcon.SRV(), buttonSize))
		{

		}

		ImGui::SameLine();

		if (ImGui::ImageButton(m_PauseIcon.SRV(), buttonSize))
		{

		}

		ImGui::SameLine();

		float time = 0.0f;

		auto max = ImGui::GetWindowContentRegionMax();
		auto min = ImGui::GetWindowContentRegionMin();

		auto currentCursorPos = ImGui::GetCursorPos();

		auto sliderFillWidth = max.x - currentCursorPos.x;

		auto imguiContext = ImGui::GetCurrentContext();
		auto imguiCurrentWindow = imguiContext->CurrentWindow;
		auto sliderHeight = imguiContext->FontSize;

		auto newCursorPosY = currentCursorPos.y + 0.5f * (buttonSize.y - sliderHeight);
		ImGui::SetCursorPosY(newCursorPosY);

		auto prevItemWidth = imguiCurrentWindow->DC.ItemWidth;
		imguiCurrentWindow->DC.ItemWidth = sliderFillWidth;

		ImGui::SliderFloat("###time_duration_slider", &time, 0.0f, m_SimulationTime, "%.1f s", ImGuiSliderFlags_AlwaysClamp);

		imguiCurrentWindow->DC.ItemWidth = prevItemWidth;

		ImGui::End();
	}
}
