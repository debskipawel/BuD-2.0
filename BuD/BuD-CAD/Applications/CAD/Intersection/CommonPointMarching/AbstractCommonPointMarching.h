#pragma once

#include <Applications/CAD/Intersection/Structures/IntersectionStructures.h>
#include <Applications/CAD/Intersection/CommonPointMarching/MarchingDirection.h>
#include <Applications/CAD/Intersection/Sampler/ISampler.h>

class AbstractCommonPointMarching
{
public:
	AbstractCommonPointMarching(std::weak_ptr<SceneObjectCAD> object1, std::weak_ptr<SceneObjectCAD> object2);

	virtual NextCommonPointResult NextPoint(dxm::Vector4 startingPoint, float distance, MarchingDirection direction = MarchingDirection::FORWARD) = 0;
	virtual NextCommonPointResult NextPoint(dxm::Vector4 startingPoint, dxm::Vector3 spaceDirection, float distance) = 0;

protected:
	std::unique_ptr<ISampler> m_Sampler;

	std::weak_ptr<SceneObjectCAD> m_ParameterizedObject1;
	std::weak_ptr<SceneObjectCAD> m_ParameterizedObject2;
};