#pragma once

#include <Intersection/InitialCommonPoint/AbstractInitialCommonPointFinder.h>

class LinearApproximationCommonPointFinder : public AbstractInitialCommonPointFinder
{
public:
	LinearApproximationCommonPointFinder(std::shared_ptr<SceneObjectCAD> object1, std::shared_ptr<SceneObjectCAD> object2, float precision);

	virtual StartingCommonPointResult FindNearestCommonPoint(dxm::Vector4 startingPosition) override;

	inline virtual constexpr const char* AlgorithmName() const override { return "Linear approximation"; }

protected:
	struct LineResult
	{
		dxm::Vector3 m_PointOnLine;
		dxm::Vector3 m_Direction;

		bool m_ResultFound;
	};

	struct ProjectionResult
	{
		dxm::Vector2 m_Parameter;

		bool m_OutOfBounds;
	};

	virtual LineResult FindIntersectionBetweenPlanes(dxm::Vector3 point1, dxm::Vector3 normal1, dxm::Vector3 point2, dxm::Vector3 normal2);
	virtual ProjectionResult ProjectNewPointToParameterSpace(std::shared_ptr<SceneObjectCAD> object, dxm::Vector2 oldParameter, dxm::Vector3 newPoint);

	float m_Precision;
};
