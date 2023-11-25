#pragma once

#include <Applications/CAD/Objects/ParameterizedObject2D.h>
#include <Applications/CAD/Objects/PointBased/Surfaces/BaseBezierPatch.h>

#include <Applications/CAD/Scene/SceneCAD.h>

class BaseBezierSurface : public ParameterizedObject2D, public std::enable_shared_from_this<BaseBezierSurface>
{
public:
	BaseBezierSurface(SceneCAD& scene, uint32_t sizeU, uint32_t sizeV, bool cylinder);

	virtual bool ShouldDisplayPolygon();
	virtual void TogglePolygon(bool polygonOn);

	virtual void UpdateOwnership();

	std::vector<std::shared_ptr<BaseBezierPatch>> m_BezierPatches;
	
	uint32_t m_SizeU, m_SizeV;
	bool m_Cylinder;

	static dxm::Vector3 SELECTED_COLOR;
	static dxm::Vector3 UNSELECTED_COLOR;

protected:
	virtual std::vector<std::weak_ptr<Point>> CreateControlPointsForFlatSurface(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV, float patchSizeU, float patchSizeV) = 0;
	virtual std::vector<std::weak_ptr<Point>> CreateControlPointsForCylinder(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV, float patchSizeU, float patchSizeV) = 0;
};
