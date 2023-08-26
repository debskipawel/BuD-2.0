#pragma once

#include <memory>

#include <Scene/SceneObjectCAD.h>
#include <Intersection/Structures/IntersectionStructures.h>
#include <Intersection/Sampler/ISampler.h>

class AbstractInitialCommonPointFinder : public ISampler
{
public:
	AbstractInitialCommonPointFinder(std::shared_ptr<SceneObjectCAD> object1, std::shared_ptr<SceneObjectCAD> object2);

	virtual StartingCommonPointResult FindNearestCommonPoint(dxm::Vector4 startingPosition) = 0;

	virtual constexpr const char* AlgorithmName() const = 0;

protected:
	std::shared_ptr<SceneObjectCAD> m_ParameterizedObject1;
	std::shared_ptr<SceneObjectCAD> m_ParameterizedObject2;
};
