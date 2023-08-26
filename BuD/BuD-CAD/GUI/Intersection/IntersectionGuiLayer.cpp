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

			intersectionFinder->Find();

			const auto& intersectionCurve = intersectionFinder->Result();

			if (!intersectionCurve.empty())
			{
				std::vector<std::weak_ptr<Point>> controlPoints1;
				std::vector<std::weak_ptr<Point>> controlPoints2;

				for (auto& parameters : intersectionCurve)
				{
					m_PointOnSurfaceCalculator->SetParameter({ parameters.x, parameters.y });
					m_PointOnSurfaceCalculator->Visit(first);

					auto position = m_PointOnSurfaceCalculator->Result();

					m_PointOnSurfaceCalculator->SetParameter({ parameters.z, parameters.w });
					m_PointOnSurfaceCalculator->Visit(second);

					auto position2 = m_PointOnSurfaceCalculator->Result();

					auto controlPoint = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreatePoint(position);
					auto controlPoint2 = m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreatePoint(position2);

					controlPoints1.push_back(std::dynamic_pointer_cast<Point>(controlPoint.lock()));
					controlPoints2.push_back(std::dynamic_pointer_cast<Point>(controlPoint2.lock()));
				}

				m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateYukselInterpolatingCurveC2(controlPoints1);
				m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateYukselInterpolatingCurveC2(controlPoints2);
			}
		}

		ImGui::End();
	}
}
