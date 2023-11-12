#include "KeyframeListGuiLayer.h"

#include <imgui.h>
#include <imgui_internal.h>

KeyframeListGuiLayer::KeyframeListGuiLayer(SimulationDataLayer& simulationDataLayer)
	: m_SimulationDataLayer(simulationDataLayer)
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
}

void KeyframeListGuiLayer::DrawAddKeyframeButton()
{
	auto& keyFrames = m_SimulationDataLayer.m_KeyFrames;
	auto isDisabled = m_SimulationDataLayer.m_Running;

	auto width = ImGui::GetWindowContentRegionMax().x - ImGui::GetWindowContentRegionMin().x;

	ImGui::PushItemFlag(ImGuiItemFlags_Disabled, isDisabled);

	if (ImGui::Button("Add new keyframe", ImVec2(width, 0.0)))
	{
		keyFrames.emplace_back(m_SimulationDataLayer.m_Time);

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
		auto selected = false;

		ImGui::Selectable(name.c_str(), &selected);
	}
}
