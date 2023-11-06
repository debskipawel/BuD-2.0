#pragma once

#include <Applications/CNC/Objects/Tools/MillingTool.h>

class FlatMillingTool : public MillingTool
{
public:
	FlatMillingTool(BuD::Scene& scene, const MillingToolParameters& millingToolParameters);

	virtual void Accept(MillingToolVisitor& visitor) override;

	virtual void EnableRendering() override;
	virtual void DisableRendering() override;

	virtual dxm::Vector3 CenterPoint() const override;

	virtual dxm::Vector3 GetCuttingPointInDirection(float x, float y, dxm::Vector3 direction) override;

	virtual float LocalHeight(float x, float y) override;
	virtual dxm::Vector3 LocalNormal(float x, float y) override;
};