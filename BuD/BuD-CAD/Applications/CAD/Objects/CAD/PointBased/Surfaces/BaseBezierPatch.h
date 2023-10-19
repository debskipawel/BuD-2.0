#pragma once

#include <Applications/CAD/Objects/CAD/PointBased/PointBasedObjectCAD.h>

class BaseBezierSurface;

class BaseBezierPatch : public PointBasedObjectCAD
{
public:
	BaseBezierPatch(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints, dxm::Vector2 rangeU = { 0.0f, 1.0f }, dxm::Vector2 rangeV = { 0.0f, 1.0f });

	virtual void OnPointModify() override;

	virtual bool ShouldDisplayPolygon();
	virtual void TogglePolygon(bool polygonOn) = 0;

	virtual void SetOwnerSurface(std::weak_ptr<BaseBezierSurface> surface);

	virtual void SwitchToTrimmed() = 0;
	virtual void SwitchToInstanced() = 0;

	struct BezierPatchSegmentInstanceData
	{
		std::array<dxm::Vector3, 16> m_ControlPoints;
		dxm::Vector2 m_ParameterRangeU;
		dxm::Vector2 m_ParameterRangeV;
		dxm::Vector3 m_Color;
	};

	static dxm::Vector3 SELECTED_COLOR;
	static dxm::Vector3 UNSELECTED_COLOR;

	BezierPatchSegmentInstanceData m_InstanceData;
	dxm::Vector3 m_Color = dxm::Vector3::One;

	std::weak_ptr<BaseBezierSurface> m_OwnerSurface;

protected:
	BuD::MeshDetails LoadPatchPrimitiveMesh();
	BuD::MeshDetails LoadPolygonPrimitiveMesh();

	bool m_DisplayBezierPolygon;
};
