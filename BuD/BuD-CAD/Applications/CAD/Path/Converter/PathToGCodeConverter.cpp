#include "PathToGCodeConverter.h"

#include <Commands/ToolMove/SlowToolMoveCommand.h>
#include <Commands/UnitSystem/MillimetersUnitSystemSelectionCommand.h>
#include <Commands/ToolPositioning/ToolPositioningAbsoluteCommand.h>
#include <Commands/ProgramStop/ProgramStopCommand.h>

auto PathToGCodeConverter::Convert(const MillingToolPath& millingToolPath) -> GCP::GCodeProgram
{
    auto commands = std::vector<std::shared_ptr<GCP::GCodeCommand>>();

    commands.emplace_back(std::make_shared<GCP::ToolPositioningAbsoluteCommand>());
    commands.emplace_back(std::make_shared<GCP::MillimetersUnitSystemSelectionCommand>());

    auto previousToolPosition = dxm::Vector3(NAN);

    auto unitScale = 10.0f;

    for (const auto& millingToolPosition : millingToolPath.GetPath())
    {
        auto x = (millingToolPosition.x == previousToolPosition.x) ? std::optional<float>() : unitScale * millingToolPosition.x;
        auto y = (millingToolPosition.y == previousToolPosition.y) ? std::optional<float>() : unitScale * millingToolPosition.y;
        auto z = (millingToolPosition.z == previousToolPosition.z) ? std::optional<float>() : unitScale * millingToolPosition.z;

        previousToolPosition = millingToolPosition;

        commands.emplace_back(std::make_shared<GCP::SlowToolMoveCommand>(x, y, z, std::nullopt));
    }

    commands.emplace_back(std::make_shared<GCP::ProgramStopCommand>());

    return GCP::GCodeProgram(commands);
}
