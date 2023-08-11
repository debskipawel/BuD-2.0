#pragma once

#include <Scene/SceneObjectCAD.h>

class PointBasedObjectCAD;

class Point : public SceneObjectCAD
{
public:
	Point(BuD::Scene& scene);
	Point(BuD::Scene& scene, dxm::Vector3 position);

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void OnDelete() override;

	struct PointInstanceData
	{
		dxm::Vector3 m_Position;
	};

	PointInstanceData m_InstanceData;

	std::vector<std::weak_ptr<PointBasedObjectCAD>> m_PointBasedObjects;
};
