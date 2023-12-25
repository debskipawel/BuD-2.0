#include "IntersectionGuiLayer.h"

#include <imgui.h>

#include <Applications/CAD/Intersection/SelfIntersectionAlgorithm.h>
#include <Applications/CAD/Intersection/MultiIntersectionAlgorithm.h>
#include <Applications/CAD/Intersection/ResultConverter/IntersectionResultConverter.h>

#include <Applications/CAD/Visitors/Intersection/CalculatorPointOnSurface.h>

IntersectionGuiLayer::IntersectionGuiLayer(MainDataLayerCAD& dataLayer)
	: BaseGuiLayerCAD(dataLayer)
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
				BuD::Log::WriteInfo(std::format("Intersection detected ({}).", intersectionResult.m_LoopDetected.has_value() ? "loop" : "no loop"));

				auto resultConverter = IntersectionResultConverter();

				auto [points1, points2] = resultConverter.ToPointVectors(intersectionResult);

				m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateIntersectionCurve(first, points1);
				m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateIntersectionCurve(second, points2);
			}
			else
			{
				BuD::Log::WriteWarning("Intersection not found.");
			}
		}

		ImGui::End();
	}
}
