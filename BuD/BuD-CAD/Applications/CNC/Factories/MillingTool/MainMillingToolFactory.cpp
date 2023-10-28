#include "MainMillingToolFactory.h"

#include <Applications/CNC/Factories/MillingTool/FlatMillingToolFactory.h>
#include <Applications/CNC/Factories/MillingTool/SphericalMillingToolFactory.h>

MainMillingToolFactory::MainMillingToolFactory()
{
	m_Factories.emplace_back(std::make_unique<FlatMillingToolFactory>());
	m_Factories.emplace_back(std::make_unique<SphericalMillingToolFactory>());
}

bool MainMillingToolFactory::CanBeCreated(std::filesystem::path filepath)
{
	return std::any_of(m_Factories.begin(), m_Factories.end(),
		[filepath](const std::unique_ptr<MillingToolFactory>& factory)
		{
			return factory->CanBeCreated(filepath);
		}
	);
}

std::shared_ptr<MillingTool> MainMillingToolFactory::Create(BuD::Scene& scene, std::filesystem::path filepath)
{
	for (const auto& factory : m_Factories)
	{
		auto millingTool = factory->Create(scene, filepath);

		if (millingTool)
		{
			return millingTool;
		}
	}

	return std::shared_ptr<MillingTool>();
}
