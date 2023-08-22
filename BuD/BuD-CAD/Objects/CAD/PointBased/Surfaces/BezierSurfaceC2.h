#pragma once

#include <Scene/SceneCAD.h>

#include <Objects/CAD/PointBased/Surfaces/BaseBezierSurface.h>

class BezierSurfaceC2 : public BaseBezierSurface
{
public:
	BezierSurfaceC2(SceneCAD& scene, std::vector<std::weak_ptr<BaseBezierPatch>> patches, uint32_t sizeU, uint32_t sizeV, bool cylinder);
	BezierSurfaceC2(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV, bool cylinder = false);

	virtual void Accept(AbstractVisitor& visitor) override;

protected:
	std::vector<std::weak_ptr<Point>> CreateControlPointsForFlatSurface(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV) override;
	std::vector<std::weak_ptr<Point>> CreateControlPointsForCylinder(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV) override;
};
