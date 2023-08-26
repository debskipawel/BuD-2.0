#pragma once

#include <Intersection/Structures/IntersectionStructures.h>
#include <Intersection/CommonPointMarching/MarchingDirection.h>
#include <Intersection/Sampler/ISampler.h>

class AbstractCommonPointMarching : public ISampler
{
public:
	AbstractCommonPointMarching(std::shared_ptr<SceneObjectCAD> object1, std::shared_ptr<SceneObjectCAD> object2);

	virtual NextCommonPointResult NextPoint(dxm::Vector4 startingPoint, float distance, MarchingDirection direction = MarchingDirection::FORWARD) = 0;
	virtual NextCommonPointResult NextPoint(dxm::Vector4 startingPoint, dxm::Vector3 spaceDirection, float distance) = 0;

protected:
	std::shared_ptr<SceneObjectCAD> m_ParameterizedObject1;
	std::shared_ptr<SceneObjectCAD> m_ParameterizedObject2;
};
