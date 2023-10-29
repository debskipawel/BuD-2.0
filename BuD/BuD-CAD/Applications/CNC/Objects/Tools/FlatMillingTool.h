#pragma once

#include <Applications/CNC/Objects/Tools/MillingTool.h>

class FlatMillingTool : public MillingTool
{
public:
	FlatMillingTool(BuD::Scene& scene, const MillingToolParameters& millingToolParameters);

	virtual void EnableRendering() override;
	virtual void DisableRendering() override;

	virtual float LocalHeight(float x, float y) override;
	virtual dxm::Vector3 LocalNormal(float x, float y) override;
};
