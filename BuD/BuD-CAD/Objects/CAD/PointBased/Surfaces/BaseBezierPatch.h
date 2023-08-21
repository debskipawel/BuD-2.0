#pragma once

#include <Objects/CAD/PointBased/PointBasedObjectCAD.h>

class BaseBezierPatch : public PointBasedObjectCAD
{
public:
	BaseBezierPatch(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints);

	virtual void OnPointModify() override;

	virtual bool ShouldDisplayPolygon();
	virtual void TogglePolygon(bool polygonOn) = 0;

	struct BezierPatchSegmentInstanceData
	{
		std::array<dxm::Vector3, 16> m_ControlPoints;
		dxm::Vector3 m_Color;
	};

	static dxm::Vector3 SELECTED_COLOR;
	static dxm::Vector3 UNSELECTED_COLOR;

	BezierPatchSegmentInstanceData m_InstanceData;
	dxm::Vector3 m_Color = dxm::Vector3::One;

protected:
	BuD::MeshDetails LoadPatchPrimitiveMesh();
	BuD::MeshDetails LoadPolygonPrimitiveMesh();

	bool m_DisplayBezierPolygon;
};
