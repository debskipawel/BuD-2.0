#pragma once

#include <Applications/CAD/Scene/SceneCAD.h>

#include <Applications/CAD/Objects/PointBased/Surfaces/BaseBezierSurface.h>

class BezierSurfaceC2 : public BaseBezierSurface
{
public:
	BezierSurfaceC2(SceneCAD& scene, std::vector<std::shared_ptr<BaseBezierPatch>> patches, uint32_t sizeU, uint32_t sizeV, bool cylinder);
	BezierSurfaceC2(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV, bool cylinder = false);

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void AddIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve) override;
	virtual void RemoveIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve) override;

protected:
	std::vector<std::weak_ptr<Point>> CreateControlPointsForFlatSurface(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV) override;
	std::vector<std::weak_ptr<Point>> CreateControlPointsForCylinder(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV) override;
};
