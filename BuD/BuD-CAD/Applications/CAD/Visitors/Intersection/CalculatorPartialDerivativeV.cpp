#include "CalculatorPartialDerivativeV.h"

#include <numbers>

#include <Applications/CAD/Visitors/Intersection/CalculatorPointOnSurface.h>
#include <Applications/CAD/Visitors/Intersection/ParameterWrapperVisitor.h>

void CalculatorPartialDerivativeV::Visit(Torus& torus)
{
	auto R = torus.m_InstanceData.m_OuterRadius;
	auto r = torus.m_InstanceData.m_InnerRadius;

	auto u = m_Parameter.x;
	auto v = m_Parameter.y;

	auto pi = std::numbers::pi_v<float>;

	auto su = sinf(2.0f * pi * u), cu = cosf(2.0f * pi * u);
	auto sv = sinf(2.0f * pi * v), cv = cosf(2.0f * pi * v);

	auto localVector = dxm::Vector4{ -2.0f * pi * sv * (R - r * cu), 2.0f * pi * cv * (R + r * cu), 0.0f, 0.0f };
	auto worldVector = dxm::Vector4::Transform(localVector, torus.m_InstanceData.m_ModelMatrix);

	m_Result = { worldVector.x, worldVector.y, worldVector.z };
}

void CalculatorPartialDerivativeV::Visit(InfinitePlane& plane)
{
	m_Result = plane.GetV();
}

void CalculatorPartialDerivativeV::Visit(FinitePlane& plane)
{
	m_Result = plane.GetTransformedV();
}

void CalculatorPartialDerivativeV::Visit(BezierSurfaceC0& surface)
{
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

	std::array<dxm::Vector3, 3> vDerControlPoints =
	{
		3.0f * (vControlPoints[1] - vControlPoints[0]),
		3.0f * (vControlPoints[2] - vControlPoints[1]),
		3.0f * (vControlPoints[3] - vControlPoints[2]),
	};

	m_Result = DeCastiljeau2(vDerControlPoints, v) * surface.m_SizeV;
}

void CalculatorPartialDerivativeV::Visit(BezierSurfaceC2& surface)
{
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

	std::array<dxm::Vector3, 3> vDerPointsInBernstein =
	{
		3.0f * (vPointsInBernstein[1] - vPointsInBernstein[0]),
		3.0f * (vPointsInBernstein[2] - vPointsInBernstein[1]),
		3.0f * (vPointsInBernstein[3] - vPointsInBernstein[2]),
	};

	m_Result = DeCastiljeau2(vDerPointsInBernstein, v) * surface.m_SizeV;
}

void CalculatorPartialDerivativeV::Visit(OffsetSurface& surface)
{
	auto inner = surface.InternalSurface();

	auto pointCalculator = std::make_unique<CalculatorPointOnSurface>();
	auto parameterWrapper = std::make_unique<ParameterWrapperVisitor>();

	auto h = 0.0005f;
	auto mul = 1.0f;

	auto [u, v] = m_Parameter;

	auto vph = v + h;
	auto vmh = v - h;

	auto prevParameter = dxm::Vector2(u, vmh);
	auto nextParameter = dxm::Vector2(u, vph);

	parameterWrapper->SetParameter(prevParameter);
	parameterWrapper->Visit(inner);

	prevParameter = parameterWrapper->Parameter();

	if (vmh < 0.0f && !parameterWrapper->WrappedV())
	{
		mul = 2.0f;
	}

	parameterWrapper->SetParameter(nextParameter);
	parameterWrapper->Visit(inner);

	nextParameter = parameterWrapper->Parameter();

	if (vph > 1.0f && !parameterWrapper->WrappedV())
	{
		mul = 2.0f;
	}

	pointCalculator->SetParameter(prevParameter);
	pointCalculator->Visit(surface);

	auto prevPoint = pointCalculator->Result();

	pointCalculator->SetParameter(nextParameter);
	pointCalculator->Visit(surface);

	auto nextPoint = pointCalculator->Result();

	m_Result = (nextPoint - prevPoint) / (2.0f * h) * mul;
}
