#pragma once

#include <SimpleMath.h>

namespace dxm = DirectX::SimpleMath;

struct StartingCommonPointResult
{
	dxm::Vector4 m_Parameter;
	dxm::Vector3 m_Point;

	int m_IterationCount;
	bool m_ResultFound;
};

struct NextCommonPointResult
{
	dxm::Vector4 m_Parameter;
	dxm::Vector3 m_Point;

	bool m_WrappedU, m_WrappedV, m_WrappedS, m_WrappedT;

	bool m_ResultFound;
	bool m_ShouldContinue;
};

struct CommonPointSequenceResult
{
	bool m_LoopDetected;
};

struct PointMappingResult
{
	dxm::Vector2 m_Parameter;

	bool m_ResultFound;
};

struct ParameterWrapResult
{
	dxm::Vector4 m_Parameter;
	
	bool m_OutOfBounds;
	bool m_WrappedU, m_WrappedV, m_WrappedS, m_WrappedT;
};
