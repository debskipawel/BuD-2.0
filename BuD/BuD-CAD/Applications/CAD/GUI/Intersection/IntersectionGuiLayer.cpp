#include "IntersectionGuiLayer.h"

#include <imgui.h>

#include <Applications/CAD/Intersection/SelfIntersectionAlgorithm.h>
#include <Applications/CAD/Intersection/MultiIntersectionAlgorithm.h>

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

				auto converter = [](
					const IntersectionResult& result,
					std::function<dxm::Vector2(const dxm::Vector4&)> uvGetter,
					std::function<dxm::Vector2(const NextCommonPointResult&)> uvWrappedGetter,
					std::function<dxm::Vector2(const LoopResult&)> loopWrappedGetter
				)
				{
					std::vector<IntersectionPoint> intersectionPoints = {};
					dxm::Vector2 prevWrapped = { 0.0f, 0.0f };

					std::transform(
						result.m_BackwardsPoints.rbegin(), result.m_BackwardsPoints.rend(),
						std::back_inserter(intersectionPoints),
						[&prevWrapped, &uvGetter, &uvWrappedGetter](const NextCommonPointResult& point)
						{
							auto result = IntersectionPoint(uvGetter(point.m_Parameter), prevWrapped.x, prevWrapped.y);
							
							prevWrapped = -uvWrappedGetter(point);

							return result;
						}
					);

					intersectionPoints.emplace_back(uvGetter(result.m_StartingPoint.m_Parameter), prevWrapped.x, prevWrapped.y);

					std::transform(
						result.m_ForwardPoints.begin(), result.m_ForwardPoints.end(),
						std::back_inserter(intersectionPoints),
						[&uvGetter, &uvWrappedGetter](const NextCommonPointResult& point)
						{
							auto wrapped = uvWrappedGetter(point);
							return IntersectionPoint(uvGetter(point.m_Parameter), wrapped.x, wrapped.y);
						}
					);

					if (result.m_LoopDetected.has_value())
					{
						auto loopWrapped = loopWrappedGetter(result.m_LoopDetected.value());

						intersectionPoints.emplace_back(
							uvGetter(result.m_StartingPoint.m_Parameter),
							loopWrapped.x,
							loopWrapped.y
						);
					}

					return intersectionPoints;
				};

				auto intersectionPoints1 = converter(
					intersectionResult, 
					[](const dxm::Vector4& parameter) { return dxm::Vector2{ parameter.x, parameter.y }; },
					[](const NextCommonPointResult& result) { return dxm::Vector2{ result.m_WrappedU, result.m_WrappedV }; },
					[](const LoopResult& result) { return dxm::Vector2{ result.m_WrappedU, result.m_WrappedV }; }
				);

				auto intersectionPoints2 = converter(
					intersectionResult,
					[](const dxm::Vector4& parameter) { return dxm::Vector2{ parameter.z, parameter.w }; },
					[](const NextCommonPointResult& result) { return dxm::Vector2{ result.m_WrappedS, result.m_WrappedT }; },
					[](const LoopResult& result) { return dxm::Vector2{ result.m_WrappedS, result.m_WrappedT }; }
				);

				m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateIntersectionCurve(first, intersectionPoints1);
				m_MainDataLayer.m_SceneDataLayer.m_SceneCAD.CreateIntersectionCurve(second, intersectionPoints2);
			}
			else
			{
				BuD::Log::WriteWarning("Intersection not found.");
			}
		}

		ImGui::End();
	}
}
