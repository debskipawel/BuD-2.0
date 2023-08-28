#include "ParameterizedObject2D.h"

constexpr dxm::Vector4 INTERSECTION_CURVE_COLOR = { 0.0f, 0.5f, 1.0f, 1.0f };

ParameterizedObject2D::ParameterizedObject2D(BuD::Scene& scene)
	: SceneObjectCAD(scene), m_ParameterSpace(std::nullopt)
{
}

void ParameterizedObject2D::AddIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve)
{
	auto intersectionCurveShared = intersectionCurve.lock();

	if (!intersectionCurveShared)
	{
		return;
	}

	m_IntersectionCurves.emplace(intersectionCurveShared->Id(), intersectionCurve);

	auto createTexture = !m_ParameterSpace.has_value();

	if (createTexture)
	{
		m_ParameterSpace = BuD::EditableTexture(256, 256);
	}

	// Draw curve in parameter space
	m_ParameterSpace->BeginEdit();

	if (createTexture)
	{
		m_ParameterSpace->Clear({ 1.0f, 1.0f, 1.0f, 1.0f });
	}

	DrawIntersectionCurveInParameterSpace(intersectionCurveShared);

	m_ParameterSpace->EndEdit();
}

void ParameterizedObject2D::DrawIntersectionCurveInParameterSpace(std::shared_ptr<IntersectionCurve> curve)
{
	const auto& intersectionPoints = curve->IntersectionPoints();

	if (intersectionPoints.empty())
	{
		return;
	}

	for (size_t i = 0; i < intersectionPoints.size() - 1; i++)
	{
		const auto& first = intersectionPoints[i];
		const auto& second = intersectionPoints[i + 1];

		if (second.m_WrappedU == 0.0f && second.m_WrappedV == 0.0f)
		{
			m_ParameterSpace->DrawLine(first.m_Parameter.x, first.m_Parameter.y, second.m_Parameter.x, second.m_Parameter.y, INTERSECTION_CURVE_COLOR);
			continue;
		}

		auto step = second.m_Parameter - first.m_Parameter + dxm::Vector2{ second.m_WrappedU, second.m_WrappedV };

		if (second.m_WrappedU == 0.0f)
		{
			auto scale = (0.5f * (second.m_WrappedV + 1.0f) - first.m_Parameter.y) / step.y;

			auto midPoint1 = first.m_Parameter + scale * step;
			auto midPoint2 = midPoint1 - dxm::Vector2{ second.m_WrappedU, second.m_WrappedV };

			m_ParameterSpace->DrawLine(first.m_Parameter.x, first.m_Parameter.y, midPoint1.x, midPoint1.y, INTERSECTION_CURVE_COLOR);
			m_ParameterSpace->DrawLine(midPoint2.x, midPoint2.y, second.m_Parameter.x, second.m_Parameter.y, INTERSECTION_CURVE_COLOR);
		}

		if (second.m_WrappedV == 0.0f)
		{
			auto scale = (0.5f * (second.m_WrappedU + 1.0f) - first.m_Parameter.x) / step.x;

			auto midPoint1 = first.m_Parameter + scale * step;
			auto midPoint2 = midPoint1 - dxm::Vector2{ second.m_WrappedU, second.m_WrappedV };

			m_ParameterSpace->DrawLine(first.m_Parameter.x, first.m_Parameter.y, midPoint1.x, midPoint1.y, INTERSECTION_CURVE_COLOR);
			m_ParameterSpace->DrawLine(midPoint2.x, midPoint2.y, second.m_Parameter.x, second.m_Parameter.y, INTERSECTION_CURVE_COLOR);
		}
	}
}
