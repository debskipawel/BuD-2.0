#include "SteepestDescentCommonPointFinder.h"

#include <Intersection/Sampler/VisitorSampler.h>

SteepestDescentInitialCommonPointFinder::SteepestDescentInitialCommonPointFinder(
	std::weak_ptr<SceneObjectCAD> object1,
	std::weak_ptr<SceneObjectCAD> object2,
	float precision
)
	: AbstractInitialCommonPointFinder(object1, object2), m_Precision(precision)
{
	m_Sampler = std::make_unique<VisitorSampler>();
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
		P = m_Sampler->GetPoint(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		Q = m_Sampler->GetPoint(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);

		auto d = P - Q;

		distance = d.Length();

		if (distance < m_Precision)
		{
			result.m_ResultFound = true;
			break;
		}

		auto dU = m_Sampler->GetDerivativeU(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto dV = m_Sampler->GetDerivativeV(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto dS = m_Sampler->GetDerivativeU(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);
		auto dT = m_Sampler->GetDerivativeV(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);

		auto gradient = 2.0f * dxm::Vector4{ d.Dot(dU), d.Dot(dV), d.Dot(-dS), d.Dot(-dT) };

		if (gradient.Length() < m_Precision)
		{
			result.m_ResultFound = false;
			break;
		}

		float delta = max(1e-10f, 0.001f / float(std::pow(10, result.m_IterationCount / 50)));

		auto newParameter = result.m_Parameter - delta * gradient;

		auto uvWrapResult = m_Sampler->WrapParameter(m_ParameterizedObject1, newParameter.x, newParameter.y);
		auto stWrapResult = m_Sampler->WrapParameter(m_ParameterizedObject2, newParameter.z, newParameter.w);

		result.m_Parameter = { uvWrapResult.m_Parameter.x, uvWrapResult.m_Parameter.y, stWrapResult.m_Parameter.x, stWrapResult.m_Parameter.y };

		if (uvWrapResult.m_OutOfBounds || stWrapResult.m_OutOfBounds)
		{
			result.m_ResultFound = false;
			return result;
		}
	}

	result.m_Point = 0.5f * (P + Q);

	return result;
}
