#include "TimelineGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

TimelineGuiLayer::TimelineGuiLayer(MainDataLayer& mainDataLayer)
	: BaseGuiLayer(mainDataLayer)
{
	m_PlayIcon = BuD::Texture::LoadFromFile("Resources/Sprites/play_icon.png");
	m_PauseIcon = BuD::Texture::LoadFromFile("Resources/Sprites/pause_icon.png");
	m_LoopIcon = BuD::Texture::LoadFromFile("Resources/Sprites/loop_icon.png");
}

auto TimelineGuiLayer::DrawGui() -> void
{
	if (ImGui::Begin("Timeline"))
	{
		auto buttonSize = dxm::Vector2(24.0f, 24.0f);

		DrawImageButton(m_PlayIcon, [this]() { m_MainDataLayer.Start(); }, buttonSize, m_MainDataLayer.IsRunning());

		ImGui::SameLine();

		DrawImageButton(m_PauseIcon, [this]() { m_MainDataLayer.Stop(); }, buttonSize, !m_MainDataLayer.IsRunning());

		ImGui::SameLine();

		auto windowPos = ImGui::GetWindowPos();
		auto max = ImGui::GetWindowContentRegionMax();
		auto min = ImGui::GetWindowContentRegionMin();

		auto currentCursorPos = ImGui::GetCursorPos();
		auto innerSpacing = ImGui::GetStyle().ItemInnerSpacing.x;

		auto sliderFillWidth = max.x - currentCursorPos.x - (buttonSize.x + 4.0f * innerSpacing);
		auto grabSize = ImVec2(12.0f, 15.0f);

		auto imguiContext = ImGui::GetCurrentContext();
		auto imguiCurrentWindow = imguiContext->CurrentWindow;
		auto sliderHeight = imguiContext->FontSize;

		auto newCursorPosY = currentCursorPos.y + 0.5f * (buttonSize.y - sliderHeight);
		ImGui::SetCursorPosY(newCursorPosY);

		auto prevItemWidth = imguiCurrentWindow->DC.ItemWidth;
		imguiCurrentWindow->DC.ItemWidth = sliderFillWidth;

		auto& animationClip = m_MainDataLayer.m_AnimationClip;
		const auto& keyFrames = animationClip.KeyFrames();

		for (const auto& keyFrame : keyFrames)
		{
			auto timePoint = keyFrame.Time();
			auto progress = timePoint / animationClip.Duration();

			auto margin = 5.0f;
			auto x = currentCursorPos.x + windowPos.x + 0.5f * innerSpacing + 0.5f * grabSize.x + progress * (sliderFillWidth - grabSize.x - innerSpacing);
			auto yUp = newCursorPosY + windowPos.y - margin;
			auto yDown = newCursorPosY + windowPos.y + sliderHeight + margin;

			auto drawList = ImGui::GetForegroundDrawList();

			imguiCurrentWindow->DrawList->AddLine({ x, yUp }, { x, yDown }, ImGui::GetColorU32({ 0.7f, 0.7f, 0.7f, 1.0f }), 2.0f);
		}

		auto simulationTime = m_MainDataLayer.GetSimulationTime();

		if (ImGui::SliderFloat("###time_duration_slider", &simulationTime, 0.0f, animationClip.Duration(), "%.1f s", ImGuiSliderFlags_AlwaysClamp))
		{
			m_MainDataLayer.SetSimulationTime(simulationTime);
		}

		imguiCurrentWindow->DC.ItemWidth = prevItemWidth;

		ImGui::SameLine();

		ImGui::SetCursorPosY(currentCursorPos.y);

		auto color = m_MainDataLayer.IsLooped() ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered) : ImGui::GetStyleColorVec4(ImGuiCol_Button);

		ImGui::PushStyleColor(ImGuiCol_Button, color);

		if (ImGui::ImageButton(m_LoopIcon.SRV(), { buttonSize.x, buttonSize.y }))
		{
			m_MainDataLayer.ToggleLoop();
		}

		ImGui::PopStyleColor();

		ImGui::End();
	}
}

void TimelineGuiLayer::DrawImageButton(const BuD::Texture& image, std::function<void()> onClick, dxm::Vector2 buttonSize, bool disabled)
{
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disabled);

	if (ImGui::ImageButton(image.SRV(), { buttonSize.x, buttonSize.y }))
	{
		onClick();
	}

	ImGui::PopItemFlag();
}
