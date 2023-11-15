#include "SimulationGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

SimulationGuiLayer::SimulationGuiLayer(SceneDataLayer& sceneDataLayer, SimulationDataLayer& dataLayer)
	: m_SceneDataLayer(sceneDataLayer), m_SimulationDataLayer(dataLayer), m_ShowGhost(false), m_IntermediateFramesCount(0)
{
	m_PlayIcon = BuD::Texture::LoadFromFile("Resources/Sprites/play_icon.png");
	m_PauseIcon = BuD::Texture::LoadFromFile("Resources/Sprites/pause_icon.png");
	m_LoopIcon = BuD::Texture::LoadFromFile("Resources/Sprites/loop_icon.png");
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
		
		auto& animationClip = m_SimulationDataLayer.m_AnimationClip;
		auto duration = animationClip.GetDuration();

		if (ImGui::DragFloat("###animation_duration", &duration, 0.1f, 0.0f, 1000.0f, "%.1f s", ImGuiSliderFlags_AlwaysClamp))
		{
			animationClip.SetDuration(duration);
		}

		ImGui::NewLine();

		ImGui::Text("Show the ghost:");
		ImGui::SameLine();
		
		if (ImGui::Checkbox("###ghost_toggle", &m_ShowGhost))
		{
			UpdateGhostMesh();
		}

		if (m_ShowGhost)
		{
			ImGui::NewLine();

			ImGui::Text("Intermediate frames count");

			if (ImGui::DragInt("###intermediate_frames_count", reinterpret_cast<int*>(&m_IntermediateFramesCount), 1.0f, 0, 100, "%d", ImGuiSliderFlags_AlwaysClamp))
			{
				UpdateGhostMesh();
			}
		}

		ImGui::End();
	}
}

void SimulationGuiLayer::DrawSimulationTimelineGui()
{
	if (ImGui::Begin("Timeline"))
	{
		auto buttonSize = dxm::Vector2(24.0f, 24.0f);

		DrawImageButton(m_PlayIcon, [this]() { m_SimulationDataLayer.Run(); }, buttonSize, m_SimulationDataLayer.m_Running);

		ImGui::SameLine();

		DrawImageButton(m_PauseIcon, [this]() { m_SimulationDataLayer.Stop(); }, buttonSize, !m_SimulationDataLayer.m_Running);

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

		auto& animationClip = m_SimulationDataLayer.m_AnimationClip;
		const auto& keyFrames = animationClip.GetKeyFrames();

		for (const auto& keyFrame : keyFrames)
		{
			auto timePoint = keyFrame.m_TimePoint;
			auto progress = timePoint / animationClip.GetDuration();

			auto margin = 5.0f;
			auto x = currentCursorPos.x + windowPos.x + 0.5f * innerSpacing + 0.5f * grabSize.x + progress * (sliderFillWidth - grabSize.x - innerSpacing);
			auto yUp = newCursorPosY + windowPos.y - margin;
			auto yDown = newCursorPosY + windowPos.y + sliderHeight + margin;

			auto drawList = ImGui::GetForegroundDrawList();

			imguiCurrentWindow->DrawList->AddLine({ x, yUp }, { x, yDown }, ImGui::GetColorU32({ 0.7f, 0.7f, 0.7f, 1.0f }), 2.0f);
		}

		ImGui::SliderFloat("###time_duration_slider", &m_SimulationDataLayer.m_Time, 0.0f, animationClip.GetDuration(), "%.1f s", ImGuiSliderFlags_AlwaysClamp);

		imguiCurrentWindow->DC.ItemWidth = prevItemWidth;

		ImGui::SameLine();

		ImGui::SetCursorPosY(currentCursorPos.y);

		auto color = m_SimulationDataLayer.m_Looped ? ImGui::GetStyleColorVec4(ImGuiCol_ButtonHovered) : ImGui::GetStyleColorVec4(ImGuiCol_Button);

		ImGui::PushStyleColor(ImGuiCol_Button, color);

		if (ImGui::ImageButton(m_LoopIcon.SRV(), { buttonSize.x, buttonSize.y }))
		{
			m_SimulationDataLayer.m_Looped = !m_SimulationDataLayer.m_Looped;
		}

		ImGui::PopStyleColor();

		ImGui::End();
	}
}

void SimulationGuiLayer::DrawImageButton(const BuD::Texture& image, std::function<void()> onClick, dxm::Vector2 buttonSize, bool disabled)
{
	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, disabled);

	if (ImGui::ImageButton(image.SRV(), { buttonSize.x, buttonSize.y }))
	{
		onClick();
	}

	ImGui::PopItemFlag();
}

void SimulationGuiLayer::UpdateGhostMesh()
{
	if (!m_ShowGhost || m_IntermediateFramesCount == 0)
	{
		m_SceneDataLayer.m_EulerGhost.DisableRendering();
		m_SceneDataLayer.m_QuaternionGhost.DisableRendering();

		return;
	}

	m_SceneDataLayer.m_EulerGhost.EnableRendering();
	m_SceneDataLayer.m_QuaternionGhost.EnableRendering();

	auto& animationClip = m_SimulationDataLayer.m_AnimationClip;

	auto intermediateFrames = animationClip.GetIntermediateFrames(m_IntermediateFramesCount);

	auto eulerFrames = std::vector<dxm::Matrix>(intermediateFrames.size());
	auto quaternionFrames = std::vector<dxm::Matrix>(intermediateFrames.size());

	std::transform(intermediateFrames.begin(), intermediateFrames.end(), eulerFrames.begin(),
		[](const KeyFrame& frame)
		{
			auto rotation = dxm::Vector3(
				DirectX::XMConvertToRadians(frame.m_EulerAngles.x),
				DirectX::XMConvertToRadians(frame.m_EulerAngles.y),
				DirectX::XMConvertToRadians(frame.m_EulerAngles.z)
			);

			return dxm::Matrix::CreateFromYawPitchRoll(rotation) * dxm::Matrix::CreateTranslation(frame.m_Position);
		}
	);

	std::transform(intermediateFrames.begin(), intermediateFrames.end(), quaternionFrames.begin(),
		[](const KeyFrame& frame)
		{
			return dxm::Matrix::CreateFromQuaternion(frame.m_Quaternion) * dxm::Matrix::CreateTranslation(frame.m_Position);
		}
	);

	m_SceneDataLayer.m_EulerGhost.UpdateModelMatrices(eulerFrames);
	m_SceneDataLayer.m_QuaternionGhost.UpdateModelMatrices(quaternionFrames);
}
