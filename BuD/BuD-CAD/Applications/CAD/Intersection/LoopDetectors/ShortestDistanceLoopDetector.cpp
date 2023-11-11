#include "ShortestDistanceLoopDetector.h"

ShortestDistanceLoopDetector::ShortestDistanceLoopDetector(std::weak_ptr<SceneObjectCAD> object1, std::weak_ptr<SceneObjectCAD> object2, float pointDistance)
	: m_ParameterizedObject1(object1), m_ParameterizedObject2(object2), m_PointDistance(pointDistance)
{
	m_ShortestDistanceCalculator = std::make_unique<CalculatorParameterShortestDistance>();
	m_ParameterWrapper = std::make_unique<ParameterWrapperVisitor>();
}

std::optional<LoopResult> ShortestDistanceLoopDetector::DetectLoop(const NextCommonPointResult& starting, const NextCommonPointResult& previous, const NextCommonPointResult& current)
{
	auto pointDiff = current.m_Point - starting.m_Point;

	if (pointDiff.Length() > m_PointDistance)
	{
		return std::nullopt;
	}

	auto startToPrevUV = ShortestVectorBetween({ starting.m_Parameter.x, starting.m_Parameter.y }, { previous.m_Parameter.x, previous.m_Parameter.y }, m_ParameterizedObject1);
	auto startToCurrentUV = ShortestVectorBetween({ starting.m_Parameter.x, starting.m_Parameter.y }, { current.m_Parameter.x, current.m_Parameter.y }, m_ParameterizedObject1);
	auto startToPrevST = ShortestVectorBetween({ starting.m_Parameter.z, starting.m_Parameter.w }, { previous.m_Parameter.z, previous.m_Parameter.w }, m_ParameterizedObject2);
	auto startToCurrentST = ShortestVectorBetween({ starting.m_Parameter.z, starting.m_Parameter.w }, { current.m_Parameter.z, current.m_Parameter.w }, m_ParameterizedObject2);

	if (startToPrevUV.Dot(startToCurrentUV) < 0.0f || startToPrevST.Dot(startToCurrentST) < 0.0f)
	{
		auto loopResult = LoopResult{};

		auto lastStep = previous.m_Parameter - dxm::Vector4{ startToPrevUV.x, startToPrevUV.y, startToPrevST.x, startToPrevST.y };
		auto wrapResult = WrapParameter(lastStep);

		loopResult.m_WrappedU = wrapResult.m_WrappedU;
		loopResult.m_WrappedV = wrapResult.m_WrappedV;
		loopResult.m_WrappedS = wrapResult.m_WrappedS;
		loopResult.m_WrappedT = wrapResult.m_WrappedT;

		return loopResult;
	}

	return std::nullopt;
}

dxm::Vector2 ShortestDistanceLoopDetector::ShortestVectorBetween(const dxm::Vector2& point1, const dxm::Vector2& point2, std::weak_ptr<SceneObjectCAD> object)
{
	m_ShortestDistanceCalculator->SetParameters({ point1.x, point1.y }, { point2.x, point2.y });
	m_ShortestDistanceCalculator->Visit(object);

	return m_ShortestDistanceCalculator->Result();
}

ParameterWrapResult ShortestDistanceLoopDetector::WrapParameter(const dxm::Vector4& parameter)
{
	auto wrapResult = ParameterWrapResult{};
	wrapResult.m_OutOfBounds = false;

	m_ParameterWrapper->SetParameter({ parameter.x, parameter.y });
	m_ParameterWrapper->Visit(m_ParameterizedObject1);

	wrapResult.m_Parameter.x = m_ParameterWrapper->Parameter().x;
	wrapResult.m_Parameter.y = m_ParameterWrapper->Parameter().y;
	
	wrapResult.m_WrappedU = m_ParameterWrapper->WrappedU();
	wrapResult.m_WrappedV = m_ParameterWrapper->WrappedV();

	wrapResult.m_OutOfBounds |= m_ParameterWrapper->OutOfRange();

	m_ParameterWrapper->SetParameter({ parameter.z, parameter.w });
	m_ParameterWrapper->Visit(m_ParameterizedObject2);

	wrapResult.m_Parameter.z = m_ParameterWrapper->Parameter().x;
	wrapResult.m_Parameter.w = m_ParameterWrapper->Parameter().y;
	
	wrapResult.m_WrappedS = m_ParameterWrapper->WrappedU();
	wrapResult.m_WrappedT = m_ParameterWrapper->WrappedV();

	wrapResult.m_OutOfBounds |= m_ParameterWrapper->OutOfRange();

	return wrapResult;
}
