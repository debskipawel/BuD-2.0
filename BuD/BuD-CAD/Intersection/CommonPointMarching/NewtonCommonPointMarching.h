#pragma once

#include <Intersection/CommonPointMarching/AbstractCommonPointMarching.h>

class NewtonCommonPointMarching : public AbstractCommonPointMarching
{
public:
	NewtonCommonPointMarching(std::weak_ptr<SceneObjectCAD> object1, std::weak_ptr<SceneObjectCAD> object2, float precision);

	virtual NextCommonPointResult NextPoint(dxm::Vector4 startingParameter, float distance, MarchingDirection marchDirection = MarchingDirection::FORWARD) override;
	virtual NextCommonPointResult NextPoint(dxm::Vector4 startingParameter, dxm::Vector3 spaceDirection, float distance) override;

protected:
	float m_Precision;
};
