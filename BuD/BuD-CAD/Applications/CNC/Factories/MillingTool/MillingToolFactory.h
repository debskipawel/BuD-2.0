#pragma once

#include <BuD.h>

#include <Applications/CNC/Objects/Tools/MillingTool.h>

class MillingToolFactory
{
public:
	virtual bool CanBeCreated(std::filesystem::path filepath) = 0;
	virtual std::shared_ptr<MillingTool> Create(BuD::Scene& scene, std::filesystem::path filepath) = 0;
};
