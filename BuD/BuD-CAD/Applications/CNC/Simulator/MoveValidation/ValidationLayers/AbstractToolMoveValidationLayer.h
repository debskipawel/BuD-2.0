#pragma once

#include <optional>

#include <Applications/CNC/Simulator/ToolCut.h>
#include <Applications/CNC/Simulator/MoveValidation/ToolMoveValidationError.h>

class AbstractToolMoveValidationLayer
{
public:
	virtual std::optional<ToolMoveValidationError> ValidateMove(const ToolCut& toolCut) = 0;
};
