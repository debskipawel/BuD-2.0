#pragma once

#include <Applications/CNC/Objects/Tools/MillingTool.h>

class FlatMillingTool : public MillingTool
{
public:
	FlatMillingTool(BuD::Scene& scene, const MillingToolParameters& millingToolParameters);

	virtual void EnableRendering() override;
	virtual void DisableRendering() override;
};
