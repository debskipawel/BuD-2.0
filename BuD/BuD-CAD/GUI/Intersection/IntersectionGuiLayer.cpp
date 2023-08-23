#include "IntersectionGuiLayer.h"

#include <imgui.h>

#include <Intersection/SelfIntersectionAlgorithm.h>
#include <Intersection/MultiIntersectionAlgorithm.h>

#include <Visitors/Intersection/CalculatorPointOnSurface.h>

IntersectionGuiLayer::IntersectionGuiLayer(MainDataLayer& dataLayer)
	: BaseGuiLayer(dataLayer)
{
	m_PointOnSurfaceCalculator = std::make_unique<CalculatorPointOnSurface>();
}

void IntersectionGuiLayer::DrawGui()
{
	if (!m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected.EligibleForIntersection())
	{
		return;
	}

	if (ImGui::Begin("Intersection"))
	{
		const auto& allSelected = m_MainDataLayer.m_SceneDataLayer.m_ManuallySelected.AllSelected();

		ImGui::Checkbox("Use selected point", &m_IntersectionParameters.m_UseSelectedStartingPoint);
		ImGui::Separator();

		if (m_IntersectionParameters.m_UseSelectedStartingPoint)
		{
			ImGui::DragFloat("U ###u_param", &m_IntersectionParameters.m_StartingParameters.x, 0.001f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);
			ImGui::DragFloat("V ###v_param", &m_IntersectionParameters.m_StartingParameters.y, 0.001f, 0.0f, 1.0f, "%.3f", ImGuiSliderFlags_AlwaysClamp);

			ImGui::Separator();
		}

		ImGui::DragFloat("Precision ###precision", &m_IntersectionParameters.m_Precision, 0.0001f, 0.0001f, 0.1f, "%.4f", ImGuiSliderFlags_AlwaysClamp);

		auto max = ImGui::GetWindowContentRegionMax();
		auto min = ImGui::GetWindowContentRegionMin();
		
		auto& style = ImGui::GetStyle();

		auto buttonSize = ImVec2(max.x - min.x, 20.0f);
		
		auto buttonMessage = allSelected.size() == 1 ? "Find self-intersection" : "Find intersection";

		ImGui::SetCursorPosY(max.y - buttonSize.y - 2 * style.ItemInnerSpacing.y);

		if (ImGui::Button(buttonMessage, buttonSize))
		{
			std::unique_ptr<BaseIntersectionAlgorithm> intersectionFinder;

			auto selectedIter = allSelected.begin();
			auto& first = selectedIter->second;

			if (allSelected.size() > 1)
			{
				selectedIter++;
				auto& second = selectedIter->second;

				intersectionFinder = std::make_unique<MultiIntersectionAlgorithm>(m_IntersectionParameters, first, second);
			}
			else
			{
				intersectionFinder = std::make_unique<SelfIntersectionAlgorithm>(m_IntersectionParameters, first);
			}

			intersectionFinder->Find();
		}

		ImGui::End();
	}
}
