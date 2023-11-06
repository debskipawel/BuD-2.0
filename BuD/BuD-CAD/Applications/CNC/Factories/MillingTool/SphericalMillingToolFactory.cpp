#include "SphericalMillingToolFactory.h"

#include <Applications/CNC/Objects/Tools/SphericalMillingTool.h>

SphericalMillingToolFactory::SphericalMillingToolFactory()
{
}

bool SphericalMillingToolFactory::CanBeCreated(std::filesystem::path filepath)
{
	auto extension = filepath.extension().string();

	return std::regex_match(extension, m_ValidationRegex);
}

std::shared_ptr<MillingTool> SphericalMillingToolFactory::Create(BuD::Scene& scene, std::filesystem::path filepath)
{
	auto extension = filepath.extension().string();
	auto results = std::smatch();

	auto match = std::regex_search(extension, results, m_ValidationRegex);

	if (!match)
	{
		return std::shared_ptr<MillingTool>();
	}

	auto number = results[2].str();
	auto radiusInMillimeter = 0.5f * std::stof(number);
	auto radiusInCentimeter = 0.1f * radiusInMillimeter;

	auto millingToolParameters = MillingToolParameters(radiusInCentimeter, 2.0f * radiusInCentimeter);
	auto millingTool = std::make_shared<SphericalMillingTool>(scene, millingToolParameters);

	return millingTool;
}
