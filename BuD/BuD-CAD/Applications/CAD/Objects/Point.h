#pragma once

#include <Applications/CAD/Scene/SceneObjectCAD.h>

class PointBasedObjectCAD;

class Point : public SceneObjectCAD
{
public:
	Point(BuD::Scene& scene);
	Point(BuD::Scene& scene, dxm::Vector3 position);

	virtual void Accept(AbstractVisitor& visitor) override;

	struct PointInstanceData
	{
		dxm::Vector3 m_Position;
		dxm::Vector3 m_Color;
	};

	PointInstanceData m_InstanceData;

	std::vector<std::weak_ptr<PointBasedObjectCAD>> m_PointBasedObjects;

	static dxm::Vector3 SELECTED_COLOR;
	static dxm::Vector3 UNSELECTED_COLOR;
};
