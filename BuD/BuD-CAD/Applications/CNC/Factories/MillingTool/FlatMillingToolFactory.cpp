#include "FlatMillingToolFactory.h"

#include <Applications/CNC/Objects/Tools/FlatMillingTool.h>

FlatMillingToolFactory::FlatMillingToolFactory()
{
}

bool FlatMillingToolFactory::CanBeCreated(std::filesystem::path filepath)
{
    auto extension = filepath.extension().string();

    return std::regex_match(extension, m_ValidationRegex);
}

std::shared_ptr<MillingTool> FlatMillingToolFactory::Create(BuD::Scene& scene, std::filesystem::path filepath)
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

    auto millingToolParameters = MillingToolParameters(radiusInCentimeter, radiusInCentimeter);
    auto millingTool = std::make_shared<FlatMillingTool>(scene, millingToolParameters);

    return millingTool;
}
