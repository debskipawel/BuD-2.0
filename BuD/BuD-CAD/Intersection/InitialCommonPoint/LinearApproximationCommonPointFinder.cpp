#include "LinearApproximationCommonPointFinder.h"

LinearApproximationCommonPointFinder::LinearApproximationCommonPointFinder(std::weak_ptr<SceneObjectCAD> object1, std::weak_ptr<SceneObjectCAD> object2, float precision)
	: AbstractInitialCommonPointFinder(object1, object2), m_Precision(precision)
{
}

StartingCommonPointResult LinearApproximationCommonPointFinder::FindNearestCommonPoint(dxm::Vector4 startingPosition)
{
	StartingCommonPointResult result = {};

	result.m_Parameter = startingPosition;

	constexpr auto MAX_ITER = 1000;

	for (result.m_IterationCount = 0; result.m_IterationCount < MAX_ITER; result.m_IterationCount++)
	{
		auto P = GetPoint(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto Q = GetPoint(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);

		auto diff = P - Q;

		if (diff.Length() < m_Precision)
		{
			result.m_Point = 0.5f * (P + Q);

			result.m_ResultFound = true;
			break;
		}

		auto n1 = GetNormal(m_ParameterizedObject1, result.m_Parameter.x, result.m_Parameter.y);
		auto n2 = GetNormal(m_ParameterizedObject2, result.m_Parameter.z, result.m_Parameter.w);

		auto line = FindIntersectionBetweenPlanes(P, n1, Q, n2);

		if (!line.m_ResultFound)
		{
			result.m_ResultFound = false;
			break;
		}
		
		auto projectedPoint1 = line.m_PointOnLine + line.m_Direction.Dot(P - line.m_PointOnLine) * line.m_Direction;
		auto projectedPoint2 = line.m_PointOnLine + line.m_Direction.Dot(Q - line.m_PointOnLine) * line.m_Direction;

		auto newPoint = 0.5f * (projectedPoint1 + projectedPoint2);

		auto uv = ProjectNewPointToParameterSpace(m_ParameterizedObject1, { result.m_Parameter.x, result.m_Parameter.y }, newPoint);
		auto st = ProjectNewPointToParameterSpace(m_ParameterizedObject2, { result.m_Parameter.z, result.m_Parameter.w }, newPoint);

		result.m_Parameter = { uv.m_Parameter.x, uv.m_Parameter.y, st.m_Parameter.x, st.m_Parameter.y };

		if (uv.m_OutOfBounds || st.m_OutOfBounds)
		{
			result.m_ResultFound = false;
			break;
		}

		result.m_Point = newPoint;
	}

	return result;
}

LinearApproximationCommonPointFinder::LineResult LinearApproximationCommonPointFinder::FindIntersectionBetweenPlanes(
	dxm::Vector3 point1, dxm::Vector3 normal1, 
	dxm::Vector3 point2, dxm::Vector3 normal2
)
{
	LineResult result = {};

	result.m_Direction = normal1.Cross(normal2);

	if (result.m_Direction.Length() < 1e-4)
	{
		result.m_ResultFound = false;
		return result;
	}

	result.m_Direction.Normalize();

	auto toLine1 = normal1.Cross(result.m_Direction);
	auto toLine2 = normal2.Cross(result.m_Direction);

	auto det1 = toLine1.x * toLine2.y - toLine1.y * toLine2.x;
	auto det2 = toLine1.y * toLine2.z - toLine1.z * toLine2.y;
	auto det3 = toLine1.x * toLine2.z - toLine1.z * toLine2.x;

	dxm::Vector2 eqResult = {};

	if (abs(det1) >= abs(det2) && abs(det1) >= abs(det3))
	{
		auto rhs = dxm::Vector2{ point2.x - point1.x, point2.y - point1.y };
		eqResult = dxm::Vector2{ toLine2.y * rhs.x - toLine2.x * rhs.y, toLine1.x * rhs.y - toLine1.y * rhs.x } / det1;
	}
	else if (abs(det2) >= abs(det1) && abs(det2) >= abs(det3))
	{
		auto rhs = dxm::Vector2{ point2.y - point1.y, point2.z - point1.z };
		eqResult = dxm::Vector2{ toLine2.z * rhs.x - toLine2.y * rhs.y, toLine1.y * rhs.y - toLine1.z * rhs.x } / det2;
	}
	else
	{
		auto rhs = dxm::Vector2{ point2.x - point1.x, point2.z - point1.z };
		eqResult = dxm::Vector2{ toLine2.z * rhs.x - toLine2.x * rhs.y, toLine1.x * rhs.y - toLine1.z * rhs.x } / det3;
	}

	result.m_PointOnLine = point1 + eqResult.x * toLine1;
	result.m_ResultFound = true;

	return result;
}

LinearApproximationCommonPointFinder::ProjectionResult LinearApproximationCommonPointFinder::ProjectNewPointToParameterSpace(
	std::weak_ptr<SceneObjectCAD> object,
	dxm::Vector2 oldParameter, 
	dxm::Vector3 newPoint
)
{
	ProjectionResult result = {};

	auto oldPoint = GetPoint(object, oldParameter.x, oldParameter.y);
	auto diff = newPoint - oldPoint;

	auto dU = GetDerivativeU(object, oldParameter.x, oldParameter.y);
	auto dV = GetDerivativeV(object, oldParameter.x, oldParameter.y);

	dxm::Vector2 parameterDiff;

	auto uvDet1 = dU.x * dV.y - dU.y * dV.x;
	auto uvDet2 = dU.y * dV.z - dU.z * dV.y;
	auto uvDet3 = dU.x * dV.z - dU.z * dV.x;

	if (abs(uvDet1) >= abs(uvDet2) && abs(uvDet1) >= abs(uvDet3))
	{
		parameterDiff = dxm::Vector2{ dV.y * diff.x - dV.x * diff.y, dU.x * diff.y - dU.y * diff.x } / uvDet1;
	}
	else if (abs(uvDet2) >= abs(uvDet1) && abs(uvDet2) >= abs(uvDet3))
	{
		parameterDiff = dxm::Vector2{ dV.z * diff.y - dV.y * diff.z, dU.y * diff.z - dU.z * diff.y } / uvDet2;
	}
	else
	{
		parameterDiff = dxm::Vector2{ dV.z * diff.x - dV.x * diff.z, dU.x * diff.z - dU.z * diff.x } / uvDet3;
	}

	result.m_Parameter = oldParameter + parameterDiff;

	m_ParameterWrapper->SetParameter(result.m_Parameter);
	m_ParameterWrapper->Visit(object);

	result.m_OutOfBounds = m_ParameterWrapper->OutOfRange();
	result.m_Parameter = m_ParameterWrapper->Parameter();

	return result;
}
