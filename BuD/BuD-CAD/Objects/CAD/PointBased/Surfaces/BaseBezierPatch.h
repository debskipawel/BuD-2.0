#pragma once

#include <Objects/CAD/PointBased/PointBasedObjectCAD.h>

class BaseBezierPatch : public PointBasedObjectCAD
{
public:
	BaseBezierPatch(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints);

	virtual void OnPointModify() override;

	struct BezierPatchSegmentInstanceData
	{
		std::array<dxm::Vector3, 16> m_ControlPoints;
		dxm::Vector3 m_Color;
	};

	BezierPatchSegmentInstanceData m_InstanceData;
	dxm::Vector3 m_Color = dxm::Vector3::One;

protected:
	BuD::MeshDetails LoadCurvePrimitiveMesh();
};
