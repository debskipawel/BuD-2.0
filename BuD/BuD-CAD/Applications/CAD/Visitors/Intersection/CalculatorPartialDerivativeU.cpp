#include "CalculatorPartialDerivativeU.h"

#include <numbers>

void CalculatorPartialDerivativeU::Visit(Torus& torus)
{
	auto R = torus.m_InstanceData.m_OuterRadius;
	auto r = torus.m_InstanceData.m_InnerRadius;

	auto u = m_Parameter.x;
	auto v = m_Parameter.y;

	auto pi = std::numbers::pi_v<float>;

	auto su = sinf(2.0f * pi * u), cu = cosf(2.0f * pi * u);
	auto sv = sinf(2.0f * pi * v), cv = cosf(2.0f * pi * v);

	auto localVector = dxm::Vector4{ -2.0f * pi * r * su * cv,  -2.0f * pi * r * su * sv, 2.0f * pi * r * cu, 0.0f };
	auto worldVector = dxm::Vector4::Transform(localVector, torus.m_InstanceData.m_ModelMatrix);

	m_Result = { worldVector.x, worldVector.y, worldVector.z };
}

void CalculatorPartialDerivativeU::Visit(InfinitePlane& plane)
{
	m_Result = plane.GetU();
}

void CalculatorPartialDerivativeU::Visit(BezierSurfaceC0& surface)
{
	auto parameter = GetPatchParameter(surface);
	auto controlPoints = GetControlPoints(surface);

	auto u = parameter.x;
	auto v = parameter.y;

	std::array<dxm::Vector3, 4> vControlPoints0 = { controlPoints[0], controlPoints[4], controlPoints[8], controlPoints[12] };
	std::array<dxm::Vector3, 4> vControlPoints1 = { controlPoints[1], controlPoints[5], controlPoints[9], controlPoints[13] };
	std::array<dxm::Vector3, 4> vControlPoints2 = { controlPoints[2], controlPoints[6], controlPoints[10], controlPoints[14] };
	std::array<dxm::Vector3, 4> vControlPoints3 = { controlPoints[3], controlPoints[7], controlPoints[11], controlPoints[15] };

	std::array<dxm::Vector3, 4> uControlPoints =
	{
		DeCastiljeau3(vControlPoints0, v),
		DeCastiljeau3(vControlPoints1, v),
		DeCastiljeau3(vControlPoints2, v),
		DeCastiljeau3(vControlPoints3, v),
	};

	std::array<dxm::Vector3, 3> uDerControlPoints =
	{
		3.0f * (uControlPoints[1] - uControlPoints[0]),
		3.0f * (uControlPoints[2] - uControlPoints[1]),
		3.0f * (uControlPoints[3] - uControlPoints[2]),
	};

	m_Result = DeCastiljeau2(uDerControlPoints, u) * surface.m_SizeU;
}

void CalculatorPartialDerivativeU::Visit(BezierSurfaceC2& surface)
{
	auto parameter = GetPatchParameter(surface);
	auto controlPoints = GetControlPoints(surface);

	auto u = parameter.x;
	auto v = parameter.y;

	std::array<dxm::Vector3, 4> vControlPoints0 = { controlPoints[0], controlPoints[4], controlPoints[8], controlPoints[12] };
	std::array<dxm::Vector3, 4> vControlPoints1 = { controlPoints[1], controlPoints[5], controlPoints[9], controlPoints[13] };
	std::array<dxm::Vector3, 4> vControlPoints2 = { controlPoints[2], controlPoints[6], controlPoints[10], controlPoints[14] };
	std::array<dxm::Vector3, 4> vControlPoints3 = { controlPoints[3], controlPoints[7], controlPoints[11], controlPoints[15] };

	auto vPointsInBernstein0 = BSplineToBernstein(vControlPoints0);
	auto vPointsInBernstein1 = BSplineToBernstein(vControlPoints1);
	auto vPointsInBernstein2 = BSplineToBernstein(vControlPoints2);
	auto vPointsInBernstein3 = BSplineToBernstein(vControlPoints3);

	std::array<dxm::Vector3, 4> uControlPoints =
	{
		DeCastiljeau3(vPointsInBernstein0, v),
		DeCastiljeau3(vPointsInBernstein1, v),
		DeCastiljeau3(vPointsInBernstein2, v),
		DeCastiljeau3(vPointsInBernstein3, v),
	};

	auto uPointsInBernstein = BSplineToBernstein(uControlPoints);

	std::array<dxm::Vector3, 3> uDerPointsInBernstein =
	{
		3.0f * (uPointsInBernstein[1] - uPointsInBernstein[0]),
		3.0f * (uPointsInBernstein[2] - uPointsInBernstein[1]),
		3.0f * (uPointsInBernstein[3] - uPointsInBernstein[2]),
	};

	m_Result = DeCastiljeau2(uDerPointsInBernstein, u) * surface.m_SizeU;
}

void CalculatorPartialDerivativeU::Visit(OffsetSurface& surface)
{
	CalculatorParameterized::Visit(surface.InternalSurface());
}
