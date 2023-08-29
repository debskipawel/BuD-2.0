#include "SteepestDescentCommonPointFinder.h"

SteepestDescentInitialCommonPointFinder::SteepestDescentInitialCommonPointFinder(
	std::weak_ptr<SceneObjectCAD> object1,
	std::weak_ptr<SceneObjectCAD> object2,
	float precision
)
	: AbstractInitialCommonPointFinder(object1, object2), m_Precision(precision)
{
}

StartingCommonPointResult SteepestDescentInitialCommonPointFinder::FindNearestCommonPoint(dxm::Vector4 startingPosition)
{
	StartingCommonPointResult result = {};
	result.m_Parameter = startingPosition;

	dxm::Vector3 P = {}, Q = {};
	float distance = 0.0f;

	constexpr auto MAX_ITER = 100;

	for (result.m_IterationCount = 0; result.m_IterationCount <= MAX_ITER; result.m_IterationCount++)
	{
		P = GetPoint(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		Q = GetPoint(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);

		auto d = P - Q;

		distance = d.Length();

		if (distance < m_Precision)
		{
			result.m_ResultFound = true;
			break;
		}

		auto dU = GetDerivativeU(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto dV = GetDerivativeV(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto dS = GetDerivativeU(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);
		auto dT = GetDerivativeV(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);

		auto gradient = 2.0f * dxm::Vector4{ d.Dot(dU), d.Dot(dV), d.Dot(-dS), d.Dot(-dT) };

		if (gradient.Length() < m_Precision)
		{
			result.m_ResultFound = true;
			break;
		}

		float delta = max(1e-10f, 0.001f / float(std::pow(10, result.m_IterationCount / 50)));

		auto wrapResult = WrapParameter(result.m_Parameter - delta * gradient);

		result.m_Parameter = wrapResult.m_Parameter;

		if (wrapResult.m_OutOfBounds)
		{
			result.m_ResultFound = false;
			return result;
		}
	}

	result.m_Point = 0.5f * (P + Q);

	return result;
}

ParameterWrapResult SteepestDescentInitialCommonPointFinder::WrapParameter(dxm::Vector4 parameter)
{
	ParameterWrapResult result = {};

	m_ParameterWrapper->SetParameter({ parameter.x, parameter.y });
	m_ParameterWrapper->Visit(m_ParameterizedObject1);

	result.m_Parameter.x = m_ParameterWrapper->Parameter().x;
	result.m_Parameter.y = m_ParameterWrapper->Parameter().y;

	result.m_OutOfBounds |= m_ParameterWrapper->OutOfRange();

	m_ParameterWrapper->SetParameter({ parameter.z, parameter.w });
	m_ParameterWrapper->Visit(m_ParameterizedObject2);

	result.m_Parameter.z = m_ParameterWrapper->Parameter().x;
	result.m_Parameter.w = m_ParameterWrapper->Parameter().y;

	result.m_OutOfBounds |= m_ParameterWrapper->OutOfRange();

	return result;
}
