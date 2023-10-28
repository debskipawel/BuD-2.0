#pragma once

#include <Applications/CNC/Objects/Tools/MillingTool.h>

class SphericalMillingTool : public MillingTool
{
public:
	SphericalMillingTool(BuD::Scene& scene, const MillingToolParameters& millingToolParameters);

	virtual void EnableRendering() override;
	virtual void DisableRendering() override;
};
