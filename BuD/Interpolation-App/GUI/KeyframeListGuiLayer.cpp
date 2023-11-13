#include "KeyframeListGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

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
			m_FrameSelectedForEditing = selected ? keyFrame.Id() : -1;

			if (selected)
			{
				m_SimulationDataLayer.m_Time = keyFrame.m_TimePoint;
			}
		}
	}
}

void KeyframeListGuiLayer::UpdateSelectedKeyframeBasedOnTime()
{
	constexpr auto MAX_TIME_DIFFERENCE = 0.05f;

	auto& keyFrames = m_SimulationDataLayer.m_KeyFrames;
	auto currentTime = m_SimulationDataLayer.m_Time;
	
	if (m_SimulationDataLayer.m_Running || keyFrames.empty())
	{
		m_FrameSelectedForEditing = -1;

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

		m_FrameSelectedForEditing = (timeDifference < MAX_TIME_DIFFERENCE) ? previous.Id() : -1;
	}
	else
	{
		if (next == keyFrames.begin())
		{
			auto timeDifference = next->m_TimePoint - currentTime;

			m_FrameSelectedForEditing = (timeDifference < MAX_TIME_DIFFERENCE) ? next->Id() : -1;
		}
		else
		{
			auto previous = next - 1;

			auto prevTimeDifference = currentTime - previous->m_TimePoint;
			auto nextTimeDifference = next->m_TimePoint - currentTime;

			auto timeDifference = (prevTimeDifference < nextTimeDifference) ? prevTimeDifference : nextTimeDifference;
			auto keyFrameCloser = (prevTimeDifference < nextTimeDifference) ? previous : next;

			m_FrameSelectedForEditing = (timeDifference < MAX_TIME_DIFFERENCE) ? keyFrameCloser->Id() : -1;
		}
	}
}
