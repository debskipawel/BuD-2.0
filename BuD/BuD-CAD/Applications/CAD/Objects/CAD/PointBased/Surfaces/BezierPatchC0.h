#pragma once

#include <Applications/CAD/Objects/CAD/PointBased/Surfaces/BaseBezierPatch.h>

class BezierPatchC0 : public BaseBezierPatch
{
public:
	BezierPatchC0(BuD::Scene& scene, std::vector<std::weak_ptr<Point>> controlPoints, dxm::Vector2 rangeU = { 0.0f, 1.0f }, dxm::Vector2 rangeV = { 0.0f, 1.0f });

	virtual void Accept(AbstractVisitor& visitor) override;

	virtual void TogglePolygon(bool polygonOn) override;

	virtual void SwitchToTrimmed() override;
	virtual void SwitchToInstanced() override;

protected:
	virtual void CreateInstanceRenderingPass();
	virtual void CreateTrimmedRenderingPass();

	BuD::RenderingPass m_InstanceRenderingPass;
	BuD::RenderingPass m_TrimmedRenderingPass;
};
