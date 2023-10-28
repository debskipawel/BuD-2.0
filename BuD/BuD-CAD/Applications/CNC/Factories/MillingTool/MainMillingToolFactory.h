#pragma once

#include <Applications/CNC/Factories/MillingTool/MillingToolFactory.h>

class MainMillingToolFactory : public MillingToolFactory
{
public:
	MainMillingToolFactory();

	virtual bool CanBeCreated(std::filesystem::path filepath) override;
	virtual std::shared_ptr<MillingTool> Create(BuD::Scene& scene, std::filesystem::path filepath) override;

protected:
	std::vector<std::unique_ptr<MillingToolFactory>> m_Factories;
};
