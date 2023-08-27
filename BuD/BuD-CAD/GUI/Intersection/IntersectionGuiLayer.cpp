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

		ImGui::Checkbox("Use cursor as starting point", &m_IntersectionParameters.m_UseCursorAsStartingPoint);
		ImGui::Separator();

		ImGui::DragFloat("Precision ###precision", &m_IntersectionParameters.m_Precision, 0.001f, 0.0001f, 0.1f, "%.6f", ImGuiSliderFlags_AlwaysClamp);
		ImGui::DragFloat("Points distance ###distance", &m_IntersectionParameters.m_PointDistance, 0.01f, 0.001f, 1.0f, "%.4f", ImGuiSliderFlags_AlwaysClamp);

		m_IntersectionParameters.m_CursorPosition = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.m_MainCursor->GetPosition();

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
			
			std::weak_ptr<SceneObjectCAD> first, second;
			first = selectedIter->second;

			if (allSelected.size() > 1)
			{
				selectedIter++;
				second = selectedIter->second;

				intersectionFinder = std::make_unique<MultiIntersectionAlgorithm>(m_IntersectionParameters, first, second);
			}
			else
			{
				second = first;

				intersectionFinder = std::make_unique<SelfIntersectionAlgorithm>(m_IntersectionParameters, first);
			}

			auto intersectionResult = intersectionFinder->Find();

			if (intersectionResult.m_IntersectionFound)
			{
				auto& startingPoint = intersectionResult.m_StartingPoint;

				std::vector<IntersectionPoint> intersectionPoints1 = {};
				std::vector<IntersectionPoint> intersectionPoints2 = {};

				float prevWrappedU = 0.0f, prevWrappedV = 0.0f;

				std::transform(
					intersectionResult.m_BackwardsPoints.rbegin(), intersectionResult.m_BackwardsPoints.rend(),
					std::back_inserter(intersectionPoints1),
					[&prevWrappedU, &prevWrappedV](const NextCommonPointResult& point)
					{
						auto result = IntersectionPoint({ point.m_Parameter.x, point.m_Parameter.y }, prevWrappedU, prevWrappedU);
						prevWrappedU = -point.m_WrappedU;
						prevWrappedV = -point.m_WrappedV;

						return result;
					}
				);

				intersectionPoints1.emplace_back(dxm::Vector2{ startingPoint.m_Parameter.x, startingPoint.m_Parameter.y }, prevWrappedU, prevWrappedV);

				std::transform(
					intersectionResult.m_ForwardPoints.begin(), intersectionResult.m_ForwardPoints.end(),
					std::back_inserter(intersectionPoints1),
					[](const NextCommonPointResult& point)
					{
						return IntersectionPoint({ point.m_Parameter.x, point.m_Parameter.y }, point.m_WrappedU, point.m_WrappedV);
					}
				);

				if (intersectionResult.m_LoopDetected.has_value())
				{
					intersectionPoints1.emplace_back(
						dxm::Vector2{ startingPoint.m_Parameter.x, startingPoint.m_Parameter.y }, 
						intersectionResult.m_LoopDetected->m_WrappedU,
						intersectionResult.m_LoopDetected->m_WrappedV
					);
				}

				m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateIntersectionCurve(first, intersectionPoints1);
			}
			else
			{
				BuD::Log::WriteWarning("Intersection not found.");
			}
		}

		ImGui::End();
	}
}
