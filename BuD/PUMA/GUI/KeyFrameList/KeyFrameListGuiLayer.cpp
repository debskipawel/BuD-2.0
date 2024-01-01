#include "KeyFrameListGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

constexpr std::optional<int> NO_KEYFRAME_SELECTED = std::nullopt;

KeyFrameListGuiLayer::KeyFrameListGuiLayer(MainDataLayer& mainDataLayer)
    : BaseGuiLayer(mainDataLayer), m_FrameSelectedForEditing(NO_KEYFRAME_SELECTED)
{
}

auto KeyFrameListGuiLayer::DrawGui() -> void
{
	if (ImGui::Begin("Keyframes list"))
	{
		DrawAddKeyframeButton();

		ImGui::Separator();

		DrawKeyframeList();

		ImGui::End();
	}

	UpdateSelectedKeyframeBasedOnTime();
	DrawGuiForSelectedKeyframe();
}

auto KeyFrameListGuiLayer::DrawAddKeyframeButton() -> void
{
	auto isDisabled = m_MainDataLayer.IsRunning();

	auto width = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;

	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, isDisabled);

	if (ImGui::Button("Add new keyframe", ImVec2(width, 0.0)))
	{
		auto currentTime = m_MainDataLayer.GetSimulationTime();
		auto newKeyframe = AnimationKeyFrame(currentTime, RobotConfiguration());

		m_MainDataLayer.m_AnimationClip.AddKeyFrame(newKeyframe);
		m_MainDataLayer.RevalidateSimulationMeshes();
	}

	ImGui::PopItemFlag();
}

auto KeyFrameListGuiLayer::DrawKeyframeList() -> void
{
	auto& animationClip = m_MainDataLayer.m_AnimationClip;

	const auto& keyFrames = animationClip.KeyFrames();

	for (auto& keyFrame : keyFrames)
	{
		auto name = keyFrame.m_Name.empty() ? std::format("Frame {}", keyFrame.Id()) : keyFrame.m_Name;
		auto label = std::format("{} ({} s)", name, keyFrame.GetTime());

		auto selected = (keyFrame.Id() == m_FrameSelectedForEditing);

		if (ImGui::Selectable(label.c_str(), &selected) && !m_MainDataLayer.IsRunning())
		{
			if (selected)
			{
				m_FrameSelectedForEditing = keyFrame.Id();
				m_MainDataLayer.SetSimulationTime(keyFrame.GetTime());
			}
			else
			{
				m_FrameSelectedForEditing = NO_KEYFRAME_SELECTED;
			}
		}
	}
}

auto KeyFrameListGuiLayer::DrawGuiForSelectedKeyframe() -> void
{
	if (m_FrameSelectedForEditing == NO_KEYFRAME_SELECTED)
	{
		return;
	}

	auto& animationClip = m_MainDataLayer.m_AnimationClip;
	auto& keyFrames = animationClip.KeyFrames();

	auto selectedKeyFrameIt = std::find_if(keyFrames.begin(), keyFrames.end(), [this](const AnimationKeyFrame& frame) { return frame.Id() == m_FrameSelectedForEditing.value(); });

	if (selectedKeyFrameIt == keyFrames.end())
	{
		return;
	}

	auto& selectedKeyFrame = *selectedKeyFrameIt;

	ImGui::Begin("Keyframe details", nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

	ImGui::Text("Name");
	ImGui::InputText("###keyframe_name", &selectedKeyFrame.m_Name);

	ImGui::NewLine();

	ImGui::Text("Keyframe time");

	auto keyFrameTime = selectedKeyFrame.GetTime();

	if (ImGui::DragFloat("###keyframe_time", &keyFrameTime, 0.1f, 0.0f, animationClip.Duration(), "%.1f", ImGuiSliderFlags_ClampOnInput))
	{
		selectedKeyFrame.SetTime(keyFrameTime);
		animationClip.SortFrames();

		m_MainDataLayer.SetSimulationTime(keyFrameTime);
	}

	auto max = ImGui::GetWindowContentRegionMax();
	auto min = ImGui::GetWindowContentRegionMin();

	auto cursorPos = ImGui::GetCursorPos();
	auto style = ImGui::GetStyle();

	auto buttonHeight = 20;
	auto buttonHeightWithSpacing = buttonHeight + style.ItemInnerSpacing.y;
	auto fullHeight = buttonHeightWithSpacing;

	if (max.y - fullHeight > cursorPos.y)
	{
		ImGui::SetCursorPos({ min.x, max.y - fullHeight });
	}

	ImGui::Separator();

	if (ImGui::Button("Delete keyframe", ImVec2(max.x - min.x, buttonHeight)))
	{
		auto idx = selectedKeyFrameIt - keyFrames.begin();

		animationClip.RemoveKeyFrame(idx);

		m_FrameSelectedForEditing = NO_KEYFRAME_SELECTED;

		m_MainDataLayer.RevalidateSimulationMeshes();
	}

	ImGui::End();
}

auto KeyFrameListGuiLayer::UpdateSelectedKeyframeBasedOnTime() -> void
{
	constexpr auto MAX_TIME_DIFFERENCE = 0.05f;

	auto& animationClip = m_MainDataLayer.m_AnimationClip;

	const auto& keyFrames = animationClip.KeyFrames();

	auto currentTime = m_MainDataLayer.GetSimulationTime();

	if (m_FrameSelectedForEditing != NO_KEYFRAME_SELECTED)
	{
		auto selectedKeyFrameIt = std::find_if(keyFrames.begin(), keyFrames.end(), [this](const AnimationKeyFrame& frame) { return frame.Id() == m_FrameSelectedForEditing.value(); });

		if (selectedKeyFrameIt == keyFrames.end())
		{
			return;
		}

		auto& selectedKeyFrame = *selectedKeyFrameIt;

		auto timeDifference = fabsf(selectedKeyFrame.GetTime() - currentTime);

		if (timeDifference < MAX_TIME_DIFFERENCE)
		{
			return;
		}
	}

	if (m_MainDataLayer.IsRunning() || keyFrames.empty())
	{
		m_FrameSelectedForEditing = NO_KEYFRAME_SELECTED;

		return;
	}

	auto next = std::upper_bound(keyFrames.begin(), keyFrames.end(), currentTime,
		[](float value, const AnimationKeyFrame& keyframe)
		{
			return value < keyframe.GetTime();
		}
	);

	if (next == keyFrames.end())
	{
		auto& previous = keyFrames.back();

		auto timeDifference = currentTime - previous.GetTime();

		m_FrameSelectedForEditing = (timeDifference < MAX_TIME_DIFFERENCE) ? previous.Id() : NO_KEYFRAME_SELECTED;
	}
	else
	{
		if (next == keyFrames.begin())
		{
			auto timeDifference = next->GetTime() - currentTime;

			m_FrameSelectedForEditing = (timeDifference < MAX_TIME_DIFFERENCE) ? next->Id() : NO_KEYFRAME_SELECTED;
		}
		else
		{
			auto previous = next - 1;

			auto prevTimeDifference = currentTime - previous->GetTime();
			auto nextTimeDifference = next->GetTime() - currentTime;

			auto timeDifference = (prevTimeDifference < nextTimeDifference) ? prevTimeDifference : nextTimeDifference;
			auto keyFrameCloser = (prevTimeDifference < nextTimeDifference) ? previous : next;

			m_FrameSelectedForEditing = (timeDifference < MAX_TIME_DIFFERENCE) ? keyFrameCloser->Id() : NO_KEYFRAME_SELECTED;
		}
	}
}
