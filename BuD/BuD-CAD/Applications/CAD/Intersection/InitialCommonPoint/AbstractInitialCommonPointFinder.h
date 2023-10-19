#pragma once

#include <memory>

#include <Applications/CAD/Scene/SceneObjectCAD.h>
#include <Applications/CAD/Intersection/Structures/IntersectionStructures.h>
#include <Applications/CAD/Intersection/Sampler/ISampler.h>

class AbstractInitialCommonPointFinder
{
public:
	AbstractInitialCommonPointFinder(std::weak_ptr<SceneObjectCAD> object1, std::weak_ptr<SceneObjectCAD> object2);

	virtual StartingCommonPointResult FindNearestCommonPoint(dxm::Vector4 startingPosition) = 0;

	virtual constexpr const char* AlgorithmName() const = 0;

protected:
	std::unique_ptr<ISampler> m_Sampler;

	std::weak_ptr<SceneObjectCAD> m_ParameterizedObject1;
	std::weak_ptr<SceneObjectCAD> m_ParameterizedObject2;
};
