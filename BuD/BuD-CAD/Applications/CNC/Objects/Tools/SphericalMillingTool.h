#pragma once

#include <Applications/CNC/Objects/Tools/MillingTool.h>

class SphericalMillingTool : public MillingTool
{
public:
	SphericalMillingTool(BuD::Scene& scene, const MillingToolParameters& millingToolParameters);

	virtual void EnableRendering() override;
	virtual void DisableRendering() override;

	virtual dxm::Vector3 CenterPoint() const override;

	virtual dxm::Vector3 GetCuttingPointInDirection(dxm::Vector3 pointOnDiameter, dxm::Vector3 direction) override;

	virtual float LocalHeight(float x, float y) override;
	virtual dxm::Vector3 LocalNormal(float x, float y) override;
};
