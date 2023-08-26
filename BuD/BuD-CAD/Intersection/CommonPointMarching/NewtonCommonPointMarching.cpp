#include "NewtonCommonPointMarching.h"

NewtonCommonPointMarching::NewtonCommonPointMarching(std::shared_ptr<SceneObjectCAD> object1, std::shared_ptr<SceneObjectCAD> object2, float precision)
    : AbstractCommonPointMarching(object1, object2), m_Precision(precision)
{
}

NextCommonPointResult NewtonCommonPointMarching::NextPoint(dxm::Vector4 startingPoint, float distance, MarchingDirection marchDirection)
{
	NextCommonPointResult result = {};
	result.m_ShouldContinue = true;

	auto n1 = GetNormal(m_ParameterizedObject1, startingPoint.x, startingPoint.y);
	auto n2 = GetNormal(m_ParameterizedObject2, startingPoint.z, startingPoint.w);

	auto direction = marchDirection == MarchingDirection::BACKWARD ? n2.Cross(n1) : n1.Cross(n2);
	
	if (direction.Length() < 1e-4)
	{
		result.m_ShouldContinue = false;
		result.m_ResultFound = false;

		return result;
	}
	
	direction.Normalize();

	return NextPoint(startingPoint, direction, distance);
}

NextCommonPointResult NewtonCommonPointMarching::NextPoint(dxm::Vector4 startingPoint, dxm::Vector3 spaceDirection, float distance)
{
	NextCommonPointResult result = {};
	result.m_ShouldContinue = true;

	auto P0 = GetPoint(m_ParameterizedObject1, startingPoint.x, startingPoint.y);

	auto xk = dxm::Vector4::Zero;
	auto xk1 = startingPoint;
	result.m_Parameter = startingPoint;

	constexpr auto MAX_ITER = 50;

	for (int i = 0; i < MAX_ITER; i++)
	{
		xk = xk1;

		auto P = GetPoint(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto Q = GetPoint(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);

		auto dU = GetDerivativeU(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto dV = GetDerivativeV(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto dS = GetDerivativeU(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);
		auto dT = GetDerivativeV(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);

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

		result.m_Step = xk1 - startingPoint;

		auto wrapResult = WrapParameter(xk1);

		result.m_Parameter = wrapResult.m_Parameter;

		result.m_WrappedU = wrapResult.m_WrappedU;
		result.m_WrappedV = wrapResult.m_WrappedV;
		result.m_WrappedS = wrapResult.m_WrappedS;
		result.m_WrappedT = wrapResult.m_WrappedT;

		if (wrapResult.m_OutOfBounds)
		{
			result.m_ResultFound = true;
			result.m_ShouldContinue = false;

			return result;
		}
	}

	result.m_ResultFound = false;

	return result;
}

ParameterWrapResult NewtonCommonPointMarching::WrapParameter(dxm::Vector4 parameter)
{
	ParameterWrapResult result = {};

	m_ParameterWrapper->SetParameter({ parameter.x, parameter.y });
	m_ParameterWrapper->Visit(m_ParameterizedObject1);

	result.m_Parameter.x = m_ParameterWrapper->Parameter().x;
	result.m_Parameter.y = m_ParameterWrapper->Parameter().y;

	result.m_OutOfBounds |= m_ParameterWrapper->OutOfRange();
	
	result.m_WrappedU = m_ParameterWrapper->WrappedU();
	result.m_WrappedV = m_ParameterWrapper->WrappedV();

	m_ParameterWrapper->SetParameter({ parameter.z, parameter.w });
	m_ParameterWrapper->Visit(m_ParameterizedObject2);

	result.m_Parameter.z = m_ParameterWrapper->Parameter().x;
	result.m_Parameter.w = m_ParameterWrapper->Parameter().y;

	result.m_WrappedS = m_ParameterWrapper->WrappedU();
	result.m_WrappedT = m_ParameterWrapper->WrappedV();

	result.m_OutOfBounds |= m_ParameterWrapper->OutOfRange();

	return result;
}
