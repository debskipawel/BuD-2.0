#include "PerformanceGuiLayer.h"

#include <BuD.h>
#include <imgui.h>

PerformanceGuiLayer::PerformanceGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayerCAD(dataLayer)
{
}

void PerformanceGuiLayer::DrawGui()
{
	if (ImGui::Begin("Performance"))
	{
		auto frameStats = BuD::Renderer::GetLastFrameStats();
		auto frameTime = BuD::Profiler::FrameTime();

		std::string performanceText = "Frame time: " + BuD::HelperFunctions::FormatWithPrecision(frameTime, 1) + " [ms], FPS: " + BuD::HelperFunctions::FormatWithPrecision(1000.0f / frameTime, 2);
		ImGui::Text(performanceText.c_str());

		performanceText = "Draw calls: " + std::to_string(frameStats.m_DrawCalls) + ", instances drawn: " + std::to_string(frameStats.m_InstancesDrawn);
		ImGui::Text(performanceText.c_str());

		BuD::Profiler::InOrder(
			[](BuD::Profiler::ScopeNode* node, unsigned int recursionLevel, unsigned int childId, unsigned int selfId)
			{
				auto duration = BuD::HelperFunctions::FormatWithPrecision((float)node->DurationMs(), 2);
				auto text = std::format("{} [{} ms]", node->m_Name, duration);

				if (node->m_Children.size())
				{
					text += std::format("###tree_node_{}_{}_{}", recursionLevel, selfId, childId);
					return ImGui::TreeNode(text.c_str());
				}

				ImGui::Text(text.c_str());

				return false;
			},
			[](BuD::Profiler::ScopeNode* node, unsigned int recursionLevel, unsigned int childId, unsigned int selfId)
			{
				if (node->m_Children.size())
				{
					ImGui::TreePop();
				}
			}
		);

		ImGui::End();
	}
}
