#include "NewtonCommonPointMarching.h"

#include <Intersection/Sampler/VisitorSampler.h>

NewtonCommonPointMarching::NewtonCommonPointMarching(std::weak_ptr<SceneObjectCAD> object1, std::weak_ptr<SceneObjectCAD> object2, float precision)
    : AbstractCommonPointMarching(object1, object2), m_Precision(precision)
{
	m_Sampler = std::make_unique<VisitorSampler>();
}

NextCommonPointResult NewtonCommonPointMarching::NextPoint(dxm::Vector4 startingParameter, float distance, MarchingDirection marchDirection)
{
	NextCommonPointResult result = {};
	result.m_ShouldContinue = true;

	auto n1 = m_Sampler->GetNormal(m_ParameterizedObject1, startingParameter.x, startingParameter.y);
	auto n2 = m_Sampler->GetNormal(m_ParameterizedObject2, startingParameter.z, startingParameter.w);

	auto direction = marchDirection == MarchingDirection::BACKWARD ? n2.Cross(n1) : n1.Cross(n2);
	
	if (direction.Length() < 1e-4)
	{
		result.m_ShouldContinue = false;
		result.m_ResultFound = false;

		return result;
	}
	
	direction.Normalize();

	return NextPoint(startingParameter, direction, distance);
}

NextCommonPointResult NewtonCommonPointMarching::NextPoint(dxm::Vector4 startingParameter, dxm::Vector3 spaceDirection, float distance)
{
	NextCommonPointResult result = {};
	result.m_ShouldContinue = true;

	auto P0 = m_Sampler->GetPoint(m_ParameterizedObject1, startingParameter.x, startingParameter.y);

	auto xk = dxm::Vector4::Zero;
	auto xk1 = startingParameter;
	result.m_Parameter = startingParameter;

	constexpr auto MAX_ITER = 50;

	for (int i = 0; i < MAX_ITER; i++)
	{
		xk = xk1;

		auto P = m_Sampler->GetPoint(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto Q = m_Sampler->GetPoint(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);

		auto dU = m_Sampler->GetDerivativeU(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto dV = m_Sampler->GetDerivativeV(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto dS = m_Sampler->GetDerivativeU(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);
		auto dT = m_Sampler->GetDerivativeV(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);

		auto F = dxm::Vector4(P.x - Q.x, P.y - Q.y, P.z - Q.z, (P - P0).Dot(spaceDirection) - distance);

		auto norm = F.Length();

		if (norm < m_Precision)
		{
			result.m_Point = 0.5f * (P + Q);
			result.m_ResultFound = true;

			return result;
		}

		dxm::Matrix dF = dxm::Matrix
		{
			dU.x, dU.y, dU.z, dU.Dot(spaceDirection),
			dV.x, dV.y, dV.z, dV.Dot(spaceDirection),
			-dS.x, -dS.y, -dS.z, 0.0f,
			-dT.x, -dT.y, -dT.z, 0.0f
		};

		auto det = dF.Determinant();

		if (abs(det) < 1e-3f)
		{
			break;
		}

		auto b = dxm::Vector4::Transform(xk, dF) - F;

		// can be solved a little smarter I think, but this should work for now
		auto dFInv = dF.Invert();

		xk1 = dxm::Vector4::Transform(b, dFInv);

		result.m_Step = xk1 - startingParameter;

		auto uvWrapResult = m_Sampler->WrapParameter(m_ParameterizedObject1, xk1.x, xk1.y);
		auto stWrapResult = m_Sampler->WrapParameter(m_ParameterizedObject2, xk1.z, xk1.w);

		result.m_Parameter = { uvWrapResult.m_Parameter.x, uvWrapResult.m_Parameter.y, stWrapResult.m_Parameter.x, stWrapResult.m_Parameter.y };

		result.m_WrappedU = uvWrapResult.m_Wrapped.x;
		result.m_WrappedV = uvWrapResult.m_Wrapped.y;
		result.m_WrappedS = stWrapResult.m_Wrapped.x;
		result.m_WrappedT = stWrapResult.m_Wrapped.y;

		if (uvWrapResult.m_OutOfBounds || stWrapResult.m_OutOfBounds)
		{
			auto P = m_Sampler->GetPoint(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
			auto Q = m_Sampler->GetPoint(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);

			auto mid = 0.5f * (P + Q);
			auto diffFromStart = mid - P0;

			result.m_ResultFound = diffFromStart.Length() < 2.0f * distance;
			result.m_ShouldContinue = false;

			return result;
		}
	}

	result.m_ResultFound = false;

	return result;
}
