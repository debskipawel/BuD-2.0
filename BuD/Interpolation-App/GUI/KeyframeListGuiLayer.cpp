#include "KeyframeListGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

constexpr std::optional<int> NO_KEYFRAME_SELECTED = std::nullopt;

KeyframeListGuiLayer::KeyframeListGuiLayer(SimulationDataLayer& simulationDataLayer)
	: m_SimulationDataLayer(simulationDataLayer), m_FrameSelectedForEditing(NO_KEYFRAME_SELECTED)
{
}

void KeyframeListGuiLayer::DrawGui()
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

void KeyframeListGuiLayer::DrawAddKeyframeButton()
{
	auto isDisabled = m_SimulationDataLayer.m_Running;

	auto width = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;

	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, isDisabled);

	if (ImGui::Button("Add new keyframe", ImVec2(width, 0.0)))
	{
		m_SimulationDataLayer.m_AnimationClip.AddKeyFrame(m_SimulationDataLayer.m_Time);
	}

	ImGui::PopItemFlag();
}

void KeyframeListGuiLayer::DrawKeyframeList()
{
	auto& animationClip = m_SimulationDataLayer.m_AnimationClip;
	const auto& keyFrames = animationClip.GetKeyFrames();

	for (auto& keyFrame : keyFrames)
	{
		auto name = keyFrame.m_Name.empty() ? std::format("Frame {}", keyFrame.Id()) : keyFrame.m_Name;
		auto label = std::format("{} ({} s)", name, keyFrame.m_TimePoint);

		auto selected = (keyFrame.Id() == m_FrameSelectedForEditing);

		if (ImGui::Selectable(label.c_str(), &selected) && !m_SimulationDataLayer.m_Running)
		{
			if (selected)
			{
				m_FrameSelectedForEditing = keyFrame.Id();
				m_SimulationDataLayer.m_Time = keyFrame.m_TimePoint;
			}
			else
			{
				m_FrameSelectedForEditing = NO_KEYFRAME_SELECTED;
			}
		}
	}
}

void KeyframeListGuiLayer::DrawGuiForSelectedKeyframe()
{
	if (m_FrameSelectedForEditing == NO_KEYFRAME_SELECTED)
	{
		return;
	}

	auto& animationClip = m_SimulationDataLayer.m_AnimationClip;

	auto& selectedKeyFrame = animationClip.GetKeyFrame(m_FrameSelectedForEditing.value());

	ImGui::Begin("Keyframe details", nullptr, ImGuiWindowFlags_NoFocusOnAppearing);

	ImGui::Text("Name");
	ImGui::InputText("###keframe_name", &selectedKeyFrame.m_Name);

	ImGui::NewLine();

	ImGui::Text("Position");
	ImGui::DragFloat3("###keyframe_position", reinterpret_cast<float*>(&selectedKeyFrame.m_Position), 0.1f, 0.0f, 0.0f, "%.1f");

	ImGui::NewLine();

	ImGui::Text("Euler angles");

	if (ImGui::DragFloat3("###keyframe_euler_angles", reinterpret_cast<float*>(&selectedKeyFrame.m_EulerAngles), 1.0f, 0.0f, 0.0f, "%.1f"))
	{
		auto eulerInRadians = dxm::Vector3(
			DirectX::XMConvertToRadians(selectedKeyFrame.m_EulerAngles.x),
			DirectX::XMConvertToRadians(selectedKeyFrame.m_EulerAngles.y),
			DirectX::XMConvertToRadians(selectedKeyFrame.m_EulerAngles.z)
		);

		selectedKeyFrame.m_Quaternion = dxm::Quaternion::CreateFromYawPitchRoll(eulerInRadians);
	}

	ImGui::NewLine();

	ImGui::Text("Quaternion");

	if (ImGui::DragFloat4("###keyframe_quaternion", reinterpret_cast<float*>(&selectedKeyFrame.m_Quaternion), 0.1f, 0.0f, 0.0f, "%.1f"))
	{
		auto eulerAngles = selectedKeyFrame.m_Quaternion.ToEuler();

		selectedKeyFrame.m_EulerAngles = dxm::Vector3(
			DirectX::XMConvertToDegrees(eulerAngles.x),
			DirectX::XMConvertToDegrees(eulerAngles.y),
			DirectX::XMConvertToDegrees(eulerAngles.z)
		);
	}

	ImGui::NewLine();

	ImGui::Text("Keyframe time");

	if (ImGui::DragFloat("###keyframe_time", &selectedKeyFrame.m_TimePoint, 0.1f, 0.0f, animationClip.GetDuration(), "%.1f", ImGuiSliderFlags_ClampOnInput))
	{
		m_SimulationDataLayer.m_Time = selectedKeyFrame.m_TimePoint;

		animationClip.SortKeyFrames();
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
		m_SimulationDataLayer.m_AnimationClip.RemoveKeyFrame(m_FrameSelectedForEditing.value());
	}

	ImGui::End();
}

void KeyframeListGuiLayer::UpdateSelectedKeyframeBasedOnTime()
{
	constexpr auto MAX_TIME_DIFFERENCE = 0.05f;

	auto& animationClip = m_SimulationDataLayer.m_AnimationClip;
	const auto& keyFrames = animationClip.GetKeyFrames();
	auto currentTime = m_SimulationDataLayer.m_Time;
	
	if (m_FrameSelectedForEditing != NO_KEYFRAME_SELECTED)
	{
		auto& selectedKeyFrame = animationClip.GetKeyFrame(m_FrameSelectedForEditing.value());

		auto timeDifference = fabsf(selectedKeyFrame.m_TimePoint - currentTime);

		if (timeDifference < MAX_TIME_DIFFERENCE)
		{
			return;
		}
	}

	if (m_SimulationDataLayer.m_Running || keyFrames.empty())
	{
		m_FrameSelectedForEditing = NO_KEYFRAME_SELECTED;

		return;
	}

	auto next = std::upper_bound(keyFrames.begin(), keyFrames.end(), currentTime,
		[](float value, const KeyFrame& keyframe)
		{
			return value < keyframe.m_TimePoint;
		}
	);

	if (next == keyFrames.end())
	{
		auto& previous = keyFrames.back();

		auto timeDifference = currentTime - previous.m_TimePoint;

		m_FrameSelectedForEditing = (timeDifference < MAX_TIME_DIFFERENCE) ? previous.Id() : NO_KEYFRAME_SELECTED;
	}
	else
	{
		if (next == keyFrames.begin())
		{
			auto timeDifference = next->m_TimePoint - currentTime;

			m_FrameSelectedForEditing = (timeDifference < MAX_TIME_DIFFERENCE) ? next->Id() : NO_KEYFRAME_SELECTED;
		}
		else
		{
			auto previous = next - 1;

			auto prevTimeDifference = currentTime - previous->m_TimePoint;
			auto nextTimeDifference = next->m_TimePoint - currentTime;

			auto timeDifference = (prevTimeDifference < nextTimeDifference) ? prevTimeDifference : nextTimeDifference;
			auto keyFrameCloser = (prevTimeDifference < nextTimeDifference) ? previous : next;

			m_FrameSelectedForEditing = (timeDifference < MAX_TIME_DIFFERENCE) ? keyFrameCloser->Id() : NO_KEYFRAME_SELECTED;
		}
	}
}
