#include "PathToGCodeConverter.h"

#include <Commands/CutterCompensation/CutterCompensationDisableCommand.h>
#include <Commands/UnitSystem/MillimetersUnitSystemSelectionCommand.h>

#include <Commands/ToolMove/SlowToolMoveCommand.h>
#include <Commands/ToolPositioning/ToolPositioningAbsoluteCommand.h>
#include <Commands/ToolSpin/ToolSpinClockwiseEnableCommand.h>

#include <Commands/ProgramStop/ProgramStopCommand.h>

auto PathToGCodeConverter::Convert(const MillingToolPath& millingToolPath) -> GCP::GCodeProgram
{
    auto commandsCount = 0U;
    
    auto commands = std::vector<std::shared_ptr<GCP::GCodeCommand>>();

    commands.emplace_back(std::make_shared<GCP::CutterCompensationDisableCommand>(++commandsCount));
    commands.emplace_back(std::make_shared<GCP::ToolPositioningAbsoluteCommand>(++commandsCount));
    commands.emplace_back(std::make_shared<GCP::MillimetersUnitSystemSelectionCommand>(++commandsCount));

    commands.emplace_back(std::make_shared<GCP::ToolSpinClockwiseEnableCommand>(++commandsCount, 10000.0f));

    auto previousToolPosition = dxm::Vector3(NAN);

    auto unitScale = 10.0f;

    for (const auto& millingToolPosition : millingToolPath.GetPath())
    {
        auto x = (millingToolPosition.x == previousToolPosition.x) ? std::optional<float>() : unitScale * millingToolPosition.x;
        auto y = (millingToolPosition.y == previousToolPosition.y) ? std::optional<float>() : unitScale * millingToolPosition.y;
        auto z = (millingToolPosition.z == previousToolPosition.z) ? std::optional<float>() : unitScale * millingToolPosition.z;

        previousToolPosition = millingToolPosition;

        commands.emplace_back(std::make_shared<GCP::SlowToolMoveCommand>(++commandsCount, x, y, z, std::nullopt));
    }

    commands.emplace_back(std::make_shared<GCP::ProgramStopCommand>(++commandsCount));

    return GCP::GCodeProgram(commands);
}
