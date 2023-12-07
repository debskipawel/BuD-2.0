#include "CalculatorPointOnSurface.h"

#include <numbers>

#include <Applications/CAD/Visitors/Intersection/CalculatorNormalVector.h>

void CalculatorPointOnSurface::Visit(Torus& torus)
{
	WrapParameter(true, true);

	auto R = torus.m_InstanceData.m_OuterRadius;
	auto r = torus.m_InstanceData.m_InnerRadius;

	auto u = m_Parameter.x;
	auto v = m_Parameter.y;

	auto pi = std::numbers::pi_v<float>;

	auto su = sinf(2.0f * pi * u), cu = cosf(2.0f * pi * u);
	auto sv = sinf(2.0f * pi * v), cv = cosf(2.0f * pi * v);

	auto localPosition = dxm::Vector4{ (R + r * cu) * cv, (R + r * cu) * sv, r * su, 1.0f };

	auto worldPosition = dxm::Vector4::Transform(localPosition, torus.m_InstanceData.m_ModelMatrix);

	m_Result = { worldPosition.x, worldPosition.y, worldPosition.z };
}

void CalculatorPointOnSurface::Visit(BezierSurfaceC0& surface)
{
	WrapParameter(surface.m_Cylinder, false);

	auto parameter = GetPatchParameter(surface);
	auto controlPoints = GetControlPoints(surface);

	auto u = parameter.x;
	auto v = parameter.y;

	std::array<dxm::Vector3, 4> uControlPoints0 = { controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3] };
	std::array<dxm::Vector3, 4> uControlPoints1 = { controlPoints[4], controlPoints[5], controlPoints[6], controlPoints[7] };
	std::array<dxm::Vector3, 4> uControlPoints2 = { controlPoints[8], controlPoints[9], controlPoints[10], controlPoints[11] };
	std::array<dxm::Vector3, 4> uControlPoints3 = { controlPoints[12], controlPoints[13], controlPoints[14], controlPoints[15] };

	std::array<dxm::Vector3, 4> vControlPoints =
	{
		DeCastiljeau3(uControlPoints0, u),
		DeCastiljeau3(uControlPoints1, u),
		DeCastiljeau3(uControlPoints2, u),
		DeCastiljeau3(uControlPoints3, u),
	};

	m_Result = DeCastiljeau3(vControlPoints, v);
}

void CalculatorPointOnSurface::Visit(BezierSurfaceC2& surface)
{
	WrapParameter(surface.m_Cylinder, false);

	auto parameter = GetPatchParameter(surface);
	auto controlPoints = GetControlPoints(surface);

	auto u = parameter.x;
	auto v = parameter.y;

	std::array<dxm::Vector3, 4> uControlPoints0 = { controlPoints[0], controlPoints[1], controlPoints[2], controlPoints[3] };
	std::array<dxm::Vector3, 4> uControlPoints1 = { controlPoints[4], controlPoints[5], controlPoints[6], controlPoints[7] };
	std::array<dxm::Vector3, 4> uControlPoints2 = { controlPoints[8], controlPoints[9], controlPoints[10], controlPoints[11] };
	std::array<dxm::Vector3, 4> uControlPoints3 = { controlPoints[12], controlPoints[13], controlPoints[14], controlPoints[15] };

	auto uPointsInBernstein0 = BSplineToBernstein(uControlPoints0);
	auto uPointsInBernstein1 = BSplineToBernstein(uControlPoints1);
	auto uPointsInBernstein2 = BSplineToBernstein(uControlPoints2);
	auto uPointsInBernstein3 = BSplineToBernstein(uControlPoints3);

	std::array<dxm::Vector3, 4> vControlPoints =
	{
		DeCastiljeau3(uPointsInBernstein0, u),
		DeCastiljeau3(uPointsInBernstein1, u),
		DeCastiljeau3(uPointsInBernstein2, u),
		DeCastiljeau3(uPointsInBernstein3, u),
	};

	auto vPointsInBernstein = BSplineToBernstein(vControlPoints);

	m_Result = DeCastiljeau3(vPointsInBernstein, v);
}

void CalculatorPointOnSurface::Visit(OffsetSurface& surface)
{
	CalculatorParameterized::Visit(surface.InternalSurface());

	auto normalCalculator = std::make_unique<CalculatorNormalVector>();
	normalCalculator->Visit(surface.InternalSurface());

	m_Result += surface.Offset() * normalCalculator->Result();
}
