#pragma once

#include <Objects/CAD/PointBased/Surfaces/BaseBezierPatch.h>

class BaseBezierSurface : public PointBasedObjectCAD
{
public:
	BaseBezierSurface(BuD::Scene& scene, uint32_t sizeU, uint32_t sizeV, bool cylinder);

	virtual bool ShouldDisplayPolygon();
	virtual void TogglePolygon(bool polygonOn);

	std::vector<std::weak_ptr<BaseBezierPatch>> m_BezierPatches;
	
	uint32_t m_SizeU, m_SizeV;
	bool m_Cylinder;

	static dxm::Vector3 SELECTED_COLOR;
	static dxm::Vector3 UNSELECTED_COLOR;
};
