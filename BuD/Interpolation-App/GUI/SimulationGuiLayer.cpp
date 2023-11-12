#include "SimulationGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

SimulationGuiLayer::SimulationGuiLayer(SimulationDataLayer& dataLayer)
	: m_Running(false), m_SimulationDataLayer(dataLayer)
{
	m_PlayIcon = BuD::Texture::LoadFromFile("Resources/Sprites/play_icon.png");
	m_PauseIcon = BuD::Texture::LoadFromFile("Resources/Sprites/pause_icon.png");
}

void SimulationGuiLayer::DrawGui()
{
	DrawSimulationSettingsGui();
	DrawSimulationTimelineGui();
}

void SimulationGuiLayer::DrawSimulationSettingsGui()
{
	if (ImGui::Begin("Simulation settings"))
	{
		ImGui::Text("Time duration");
		
		ImGui::DragFloat("###animation_duration", &m_SimulationDataLayer.m_Duration, 0.1f, 0.0f, 1000.0f, "%.1f s", ImGuiSliderFlags_AlwaysClamp);

		ImGui::NewLine();

		ImGui::Text("Simulation looped");
		ImGui::SameLine();
		ImGui::Checkbox("###looped_checkbox", &m_SimulationDataLayer.m_Looped);

		ImGui::End();
	}
}

void SimulationGuiLayer::DrawSimulationTimelineGui()
{
	if (ImGui::Begin("Timeline"))
	{
		auto buttonSize = dxm::Vector2(20.0f, 20.0f);

		DrawImageButton(m_PlayIcon, [this]() { m_SimulationDataLayer.Run(); }, buttonSize, m_SimulationDataLayer.m_Running);

		ImGui::SameLine();

		DrawImageButton(m_PauseIcon, [this]() { m_SimulationDataLayer.Stop(); }, buttonSize, !m_SimulationDataLayer.m_Running);

		ImGui::SameLine();

		auto windowPos = ImGui::GetWindowPos();
		auto max = ImGui::GetWindowContentRegionMax();
		auto min = ImGui::GetWindowContentRegionMin();

		auto currentCursorPos = ImGui::GetCursorPos();

		auto sliderFillWidth = max.x - currentCursorPos.x;

		auto imguiContext = ImGui::GetCurrentContext();
		auto imguiCurrentWindow = imguiContext->CurrentWindow;
		auto sliderHeight = imguiContext->FontSize;

		auto grabSize = ImVec2(12.0f, 15.0f);
		auto innerSpacing = ImGui::GetStyle().ItemInnerSpacing.x;

		auto newCursorPosY = currentCursorPos.y + 0.5f * (buttonSize.y - sliderHeight);
		ImGui::SetCursorPosY(newCursorPosY);

		auto prevItemWidth = imguiCurrentWindow->DC.ItemWidth;
		imguiCurrentWindow->DC.ItemWidth = sliderFillWidth;

		const auto& keyFrames = m_SimulationDataLayer.m_KeyFrames;

		for (const auto& keyFrame : keyFrames)
		{
			auto timePoint = keyFrame.m_TimePoint;
			auto progress = timePoint / m_SimulationDataLayer.m_Duration;

			auto margin = 5.0f;
			auto x = currentCursorPos.x + windowPos.x + 0.5f * innerSpacing + 0.5f * grabSize.x + progress * (sliderFillWidth - grabSize.x - innerSpacing);
			auto yUp = newCursorPosY + windowPos.y - margin;
			auto yDown = newCursorPosY + windowPos.y + sliderHeight + margin;

			auto drawList = ImGui::GetForegroundDrawList();

			imguiCurrentWindow->DrawList->AddLine({ x, yUp }, { x, yDown }, ImGui::GetColorU32({ 0.7f, 0.7f, 0.7f, 1.0f }), 2.0f);
		}

		ImGui::SliderFloat("###time_duration_slider", &m_SimulationDataLayer.m_Time, 0.0f, m_SimulationDataLayer.m_Duration, "%.1f s", ImGuiSliderFlags_AlwaysClamp);

		imguiCurrentWindow->DC.ItemWidth = prevItemWidth;

		ImGui::End();
	}
}

void SimulationGuiLayer::DrawImageButton(const BuD::Texture& image, std::function<void()> onClick, dxm::Vector2 buttonSize, bool disabled)
{
	auto color = disabled ? ImVec4(0.4f, 0.1f, 0.1f, 1.0f) : ImVec4(0.8f, 0.2f, 0.2f, 1.0f);

	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disabled);

	if (ImGui::ImageButton(image.SRV(), { buttonSize.x, buttonSize.y }, { 0, 0 }, { 1, 1 }, -1, color))
	{
		onClick();
	}

	ImGui::PopItemFlag();
}