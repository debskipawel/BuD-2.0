#pragma once

#include <Scene/SceneCAD.h>

#include <Objects/CAD/PointBased/Surfaces/BaseBezierSurface.h>

class BezierSurfaceC2 : public BaseBezierSurface
{
public:
	BezierSurfaceC2(SceneCAD& scene, dxm::Vector3 position, uint32_t sizeU, uint32_t sizeV, bool cylinder = false);

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void OnPointModify() override;
};
