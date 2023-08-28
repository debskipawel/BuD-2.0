#pragma once

#include <Objects/CAD/PointBased/Surfaces/BaseBezierPatch.h>

class BezierPatchC0 : public BaseBezierPatch
{
public:
	BezierPatchC0(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints);

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void TogglePolygon(bool polygonOn) override;

protected:
	virtual void CreateInstanceRenderingPass();
	virtual void CreateTrimmedRenderingPass();

	BuD::RenderingPass m_InstanceRenderingPass;
	BuD::RenderingPass m_TrimmedRenderingPass;
};
