#include "IntersectionResultConverter.h"

#include <algorithm>
#include <functional>
#include <iterator>

auto IntersectionResultConverter::ToPointVectors(const IntersectionResult& result) -> std::pair<std::vector<IntersectionPoint>, std::vector<IntersectionPoint>>
{
	if (result.m_IntersectionFound)
	{
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
			result,
			[](const dxm::Vector4& parameter) { return dxm::Vector2{ parameter.x, parameter.y }; },
			[](const NextCommonPointResult& result) { return dxm::Vector2{ result.m_WrappedU, result.m_WrappedV }; },
			[](const LoopResult& result) { return dxm::Vector2{ result.m_WrappedU, result.m_WrappedV }; }
		);

		auto intersectionPoints2 = converter(
			result,
			[](const dxm::Vector4& parameter) { return dxm::Vector2{ parameter.z, parameter.w }; },
			[](const NextCommonPointResult& result) { return dxm::Vector2{ result.m_WrappedS, result.m_WrappedT }; },
			[](const LoopResult& result) { return dxm::Vector2{ result.m_WrappedS, result.m_WrappedT }; }
		);

		return { intersectionPoints1, intersectionPoints2 };
	}

	return { std::vector<IntersectionPoint>(), std::vector<IntersectionPoint>() };
}
