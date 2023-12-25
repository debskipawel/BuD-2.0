#pragma once

#include <Applications/CAD/Scene/SceneCAD.h>

#include <Applications/CAD/Objects/PointBased/Surfaces/BaseBezierSurface.h>

class BezierSurfaceC0 : public BaseBezierSurface
{
public:
	BezierSurfaceC0(SceneCAD& scene, std::vector<std::shared_ptr<BaseBezierPatch>> patches, uint32_t sizeU, uint32_t sizeV, float patchSizeU = 1.0f, float patchSizeV = 1.0f, bool cylinder = false);
	BezierSurfaceC0(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV, float patchSizeU = 1.0f, float patchSizeV = 1.0f, bool cylinder = false);

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void AddIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve) override;
	virtual void RemoveIntersectionCurve(std::weak_ptr<IntersectionCurve> intersectionCurve) override;

protected:
	std::vector<std::weak_ptr<Point>> CreateControlPointsForFlatSurface(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV, float patchSizeU, float patchSizeV) override;
	std::vector<std::weak_ptr<Point>> CreateControlPointsForCylinder(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV, float patchSizeU, float patchSizeV) override;
};
