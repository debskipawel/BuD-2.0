#include "KeyframeListGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

constexpr auto NO_KEYFRAME_SELECTED = -1;

KeyframeListGuiLayer::KeyframeListGuiLayer(SimulationDataLayer& simulationDataLayer)
	: m_SimulationDataLayer(simulationDataLayer), m_FrameSelectedForEditing(-1)
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
	auto& keyFrames = m_SimulationDataLayer.m_KeyFrames;
	auto isDisabled = m_SimulationDataLayer.m_Running;

	auto width = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;

	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, isDisabled);

	if (ImGui::Button("Add new keyframe", ImVec2(width, 0.0)))
	{
		auto newKeyframe = m_SimulationDataLayer.Interpolate();

		keyFrames.push_back(newKeyframe);

		std::sort(keyFrames.begin(), keyFrames.end(),
			[](const KeyFrame& frame1, const KeyFrame& frame2)
			{
				return frame1.m_TimePoint < frame2.m_TimePoint;
			}
		);
	}

	ImGui::PopItemFlag();
}

void KeyframeListGuiLayer::DrawKeyframeList()
{
	auto& keyFrames = m_SimulationDataLayer.m_KeyFrames;

	for (auto& keyFrame : keyFrames)
	{
		auto name = std::format("Frame {} ({} s)", keyFrame.Id(), keyFrame.m_TimePoint);
		auto selected = keyFrame.Id() == m_FrameSelectedForEditing;

		if (ImGui::Selectable(name.c_str(), &selected))
		{
			m_FrameSelectedForEditing = selected ? keyFrame.Id() : NO_KEYFRAME_SELECTED;

			if (selected)
			{
				m_SimulationDataLayer.m_Time = keyFrame.m_TimePoint;
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

	auto& keyFrames = m_SimulationDataLayer.m_KeyFrames;

	auto selectedKeyFrame = std::find_if(keyFrames.begin(), keyFrames.end(), [this](const KeyFrame& keyFrame) { return keyFrame.Id() == m_FrameSelectedForEditing; });

	if (selectedKeyFrame == keyFrames.end())
	{
		m_FrameSelectedForEditing = NO_KEYFRAME_SELECTED;
		return;
	}

	if (ImGui::Begin("Keyframe details"))
	{
		ImGui::Text("Position");
		ImGui::DragFloat3("###keyframe_position", reinterpret_cast<float*>(&selectedKeyFrame->m_Position), 0.1f, 0.0f, 0.0f, "%.1f");

		ImGui::NewLine();

		ImGui::Text("Euler angles");
		
		if (ImGui::DragFloat3("###keyframe_euler_angles", reinterpret_cast<float*>(&selectedKeyFrame->m_EulerAngles), 1.0f, 0.0f, 0.0f, "%.1f"))
		{
			auto eulerInRadians = dxm::Vector3(
				DirectX::XMConvertToRadians(selectedKeyFrame->m_EulerAngles.x),
				DirectX::XMConvertToRadians(selectedKeyFrame->m_EulerAngles.y),
				DirectX::XMConvertToRadians(selectedKeyFrame->m_EulerAngles.z)
			);

			selectedKeyFrame->m_Quaternion = dxm::Quaternion::CreateFromYawPitchRoll(eulerInRadians);
		}

		ImGui::NewLine();

		ImGui::Text("Quaternion");

		if (ImGui::DragFloat4("###keyframe_quaternion", reinterpret_cast<float*>(&selectedKeyFrame->m_Quaternion), 0.1f, 0.0f, 0.0f, "%.1f"))
		{
			auto eulerAngles = selectedKeyFrame->m_Quaternion.ToEuler();

			selectedKeyFrame->m_EulerAngles = dxm::Vector3(
				DirectX::XMConvertToDegrees(eulerAngles.x),
				DirectX::XMConvertToDegrees(eulerAngles.y),
				DirectX::XMConvertToDegrees(eulerAngles.z)
			);
		}

		ImGui::NewLine();

		ImGui::Text("Keyframe time");
		
		if (ImGui::DragFloat("###keyframe_time", &selectedKeyFrame->m_TimePoint, 0.1f, 0.0f, m_SimulationDataLayer.m_Duration, "%.1f", ImGuiSliderFlags_ClampOnInput))
		{
			m_SimulationDataLayer.m_Time = selectedKeyFrame->m_TimePoint;

			std::sort(keyFrames.begin(), keyFrames.end(),
				[](const KeyFrame& frame1, const KeyFrame& frame2)
				{
					return frame1.m_TimePoint < frame2.m_TimePoint;
				}
			);
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
			std::erase_if(keyFrames, [this](const KeyFrame& keyFrame) { return keyFrame.Id() == m_FrameSelectedForEditing; });
			
			m_FrameSelectedForEditing = NO_KEYFRAME_SELECTED;
		}

		ImGui::End();
	}
}

void KeyframeListGuiLayer::UpdateSelectedKeyframeBasedOnTime()
{
	constexpr auto MAX_TIME_DIFFERENCE = 0.05f;

	auto& keyFrames = m_SimulationDataLayer.m_KeyFrames;
	auto currentTime = m_SimulationDataLayer.m_Time;
	
	if (m_FrameSelectedForEditing != NO_KEYFRAME_SELECTED)
	{
		auto selectedKeyFrame = std::find_if(keyFrames.begin(), keyFrames.end(), [this](const KeyFrame& keyFrame) { return keyFrame.Id() == m_FrameSelectedForEditing; });

		if (selectedKeyFrame != keyFrames.end())
		{
			auto timeDifference = fabsf(selectedKeyFrame->m_TimePoint - currentTime);

			if (timeDifference < MAX_TIME_DIFFERENCE)
			{
				return;
			}
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
