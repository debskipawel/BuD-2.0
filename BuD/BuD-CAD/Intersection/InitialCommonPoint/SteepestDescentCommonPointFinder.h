#pragma once

#include <Intersection/InitialCommonPoint/AbstractInitialCommonPointFinder.h>

class SteepestDescentInitialCommonPointFinder : public AbstractInitialCommonPointFinder
{
public:
	SteepestDescentInitialCommonPointFinder(std::weak_ptr<SceneObjectCAD> object1, std::weak_ptr<SceneObjectCAD> object2, float precision);

	virtual StartingCommonPointResult FindNearestCommonPoint(dxm::Vector4 startingPosition) override;

	inline virtual constexpr const char* AlgorithmName() const override { return "Steepest descent"; }

protected:
	float m_Precision;
};
