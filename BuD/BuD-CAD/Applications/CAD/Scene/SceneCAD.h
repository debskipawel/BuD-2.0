#pragma once

#include <BuD.h>

#include "SceneObjectCAD.h"

#include <Applications/CAD/Objects/Additional/Cursor.h>
#include <Applications/CAD/Objects/Additional/Grid.h>

#include <Applications/CAD/Objects/CAD/Point.h>
#include <Applications/CAD/Objects/CAD/Intersection/IntersectionPoint.h>

class SceneCAD
{
public:
	SceneCAD();
	SceneCAD(const SceneCAD& other) = default;
	SceneCAD& operator=(const SceneCAD& other) = default;
	~SceneCAD();

	void Clear();
	void DeleteObject(SceneObjectCAD& object);

	std::weak_ptr<SceneObjectCAD> CreateTorus(dxm::Vector3 position);
	std::weak_ptr<SceneObjectCAD> CreatePoint(dxm::Vector3 position);
	
	std::weak_ptr<SceneObjectCAD> CreateBezierCurveC0(std::vector<std::weak_ptr<Point>> controlPoints);
	std::weak_ptr<SceneObjectCAD> CreateBezierCurveC2(std::vector<std::weak_ptr<Point>> controlPoints);
	std::weak_ptr<SceneObjectCAD> CreateYukselInterpolatingCurveC2(std::vector<std::weak_ptr<Point>> controlPoints);

	std::weak_ptr<SceneObjectCAD> CreateFlatBezierSurfaceC0(dxm::Vector3 position, uint32_t patchesU, uint32_t pachesV);
	std::weak_ptr<SceneObjectCAD> CreateCylinderBezierSurfaceC0(dxm::Vector3 position, uint32_t patchesU, uint32_t pachesV);

	std::weak_ptr<SceneObjectCAD> CreateFlatBezierSurfaceC2(dxm::Vector3 position, uint32_t patchesU, uint32_t pachesV);
	std::weak_ptr<SceneObjectCAD> CreateCylinderBezierSurfaceC2(dxm::Vector3 position, uint32_t patchesU, uint32_t pachesV);

	std::weak_ptr<SceneObjectCAD> CreateIntersectionCurve(std::weak_ptr<SceneObjectCAD> surface, std::vector<IntersectionPoint> intersectionPoints);

	std::shared_ptr<Cursor> m_MainCursor;
	std::shared_ptr<Cursor> m_CentroidCursor;

	std::map<uint32_t, std::shared_ptr<SceneObjectCAD>> m_ObjectList;

	BuD::Scene m_Scene;
	
protected:
	std::unique_ptr<Grid> m_Grid;
};
