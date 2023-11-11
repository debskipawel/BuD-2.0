#pragma once

#include <regex>

#include <Applications/CNC/Factories/MillingTool/MillingToolFactory.h>

class SphericalMillingToolFactory : public MillingToolFactory
{
public:
	SphericalMillingToolFactory();

	virtual bool CanBeCreated(std::filesystem::path filepath) override;
	virtual std::shared_ptr<MillingTool> Create(BuD::Scene& scene, std::filesystem::path filepath) override;

protected:
	std::regex m_ValidationRegex = std::regex("^\\.(k|K)([0-9]*)$");
};
